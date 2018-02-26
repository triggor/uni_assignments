/*======Projekt: System wspomagający organizację konferencji======*/
/*==========Model fizyczny. Wykonał: Igor Tryhub, 275235==========*/


CREATE TYPE rolelist AS ENUM ('organizer', 'participant');
CREATE TYPE statuslist AS ENUM ('planned', 'proposed', 'rejected', 'confirmed');
CREATE TYPE attendancelist AS ENUM ('present', 'absent');

------------------------------------------------------------------------
CREATE TABLE users (
	login varchar(20) PRIMARY KEY CHECK (login <> '' AND login ~ '^[a-zA-Z0-9_-]*$'),
	password varchar(20) NOT NULL CHECK (char_length(password) >= 4),
	role rolelist NOT NULL DEFAULT 'participant'
);

CREATE TABLE events (
	title varchar(50) PRIMARY KEY,
	creator varchar(20) NOT NULL REFERENCES users(login),
	start_time DATE NOT NULL,
	end_time DATE NOT NULL,
	CHECK (start_time <= end_time)
);

CREATE TABLE talks (
	talk_id varchar(20) PRIMARY KEY,
	event varchar(50) NULL REFERENCES events(title),
	speaker varchar(20) NOT NULL REFERENCES users(login),
	description varchar(200) NOT NULL,
	start_time TIMESTAMP WITHOUT TIME ZONE NOT NULL,
	status statuslist NOT NULL DEFAULT 'planned',
	room smallint NULL,
	coordinator varchar(20) NULL REFERENCES users(login),
	initial_eval smallint NULL,
	CHECK (initial_eval >= 0 AND initial_eval <= 10)
);

CREATE VIEW talks_view AS
	(SELECT t.talk_id, t.event, t.speaker, t.description, t.room
	FROM talks t
	WHERE status = 'planned' OR status = 'confirmed');

CREATE TABLE friendship_requests (
	participant varchar(20) NOT NULL REFERENCES users(login),
	friends_with varchar(20) NOT NULL REFERENCES users(login),
	PRIMARY KEY (participant, friends_with),
	CHECK (participant <> friends_with)
);

CREATE VIEW friends_view AS
	(SELECT fr1.participant AS friend1, fr2.participant AS friend2
	FROM friendship_requests fr1
	JOIN friendship_requests fr2 ON (fr1.friends_with=fr2.participant)
	WHERE fr1.participant=fr2.friends_with AND fr1.participant<fr2.participant);

CREATE TABLE participant_registrations (
	participant varchar(20) NOT NULL REFERENCES users(login),
	event varchar(50) NOT NULL REFERENCES events(title),
	PRIMARY KEY (participant, event)
);

CREATE TABLE talk_attendance (
	participant varchar(20) NOT NULL REFERENCES users(login),
	talk varchar(20) NOT NULL REFERENCES talks(talk_id),
	attendance attendancelist NOT NULL DEFAULT 'absent',
	PRIMARY KEY (participant, talk)
);

CREATE TABLE talk_evaluation (
	userlogin varchar(20) NOT NULL REFERENCES users(login),
	talk varchar(20) NOT NULL REFERENCES talks(talk_id),
	evaluation smallint NOT NULL,
	PRIMARY KEY (userlogin, talk),
	CHECK (evaluation >= 0 AND evaluation <= 10)
);

CREATE VIEW talk_rating_view AS
	(SELECT te.talk AS talk_id, AVG(te.evaluation) AS rating, COUNT(DISTINCT te.userlogin) AS votes
	FROM talk_evaluation te
	GROUP BY talk
	ORDER BY rating DESC);

------------------------------------------------------------------------

CREATE TYPE user_plan_type AS
	(participant varchar(20),
	talk_id varchar(20),
	start_time TIMESTAMP WITHOUT TIME ZONE,
	description varchar(200),
	room smallint
);

CREATE FUNCTION user_plan(varchar(20), int) RETURNS SETOF user_plan_type AS
$$
	SELECT pr.participant, t.talk_id, t.start_time, t.description, t.room
	FROM talks t JOIN participant_registrations pr USING(event)
	WHERE pr.participant=$1
	ORDER BY 3
	LIMIT $2;
$$ LANGUAGE SQL;

CREATE TYPE day_plan_type AS
	(talk_id varchar(20),
	start_time TIMESTAMP WITHOUT TIME ZONE,
	description varchar(200),
	room smallint
);

CREATE FUNCTION day_plan(TIMESTAMP WITHOUT TIME ZONE) RETURNS SETOF day_plan_type AS
$$
	SELECT t.talk_id, t.start_time, t.description, t.room
	FROM talks t 
	WHERE t.start_time::date = $1::date
	ORDER BY 4, 2;
$$ LANGUAGE SQL;

CREATE TYPE talks_type AS
	(talk_id varchar(20),
	start_time TIMESTAMP WITHOUT TIME ZONE,
	description varchar(200),
	room smallint
);

-- (*N) best_talks <start_timestamp> <end_timestamp> <limit> <all> 
-- zwraca referaty rozpoczynające się w  danym przedziale czasowym posortowane malejąco wg średniej oceny uczestników, 
-- przy czym jeśli <all> jest równe 1 należy wziąć pod uwagę wszystkie oceny, w przeciwnym przypadku tylko oceny uczestników, 
-- którzy byli na referacie obecni, wypisuje pierwsze <limit> referatów, przy czym 0 oznacza, że należy wypisać wszystkie
-- <talk> <start_timestamp> <title> <room>
CREATE FUNCTION best_talks(TIMESTAMP WITHOUT TIME ZONE, TIMESTAMP WITHOUT TIME ZONE, int, int)
	RETURNS SETOF talks_type AS
$$
		WITH Talks_Sorted_By_Attended_Evaluators AS
		(SELECT te.talk AS talk_id 
		FROM talk_evaluation te
		WHERE te.userlogin IN
			(SELECT te2.userlogin FROM talk_evaluation te2
			WHERE te.talk=te2.talk
			AND te2.userlogin IN
				(SELECT ta.participant FROM talk_attendance ta
				WHERE ta.attendance='present' AND ta.talk=te.talk
				)
			)
		GROUP BY talk_id
		ORDER BY AVG(te.evaluation) DESC
		)
		SELECT t.talk_id, t.start_time, t.description, t.room
		FROM Talks_Sorted_By_Attended_Evaluators JOIN talks t USING(talk_id)
		WHERE t.start_time >= $1 AND t.start_time <= $2
		LIMIT $3;
$$ LANGUAGE SQL;

CREATE FUNCTION best_talks_all(TIMESTAMP WITHOUT TIME ZONE, TIMESTAMP WITHOUT TIME ZONE, int, int)
	RETURNS SETOF talks_type AS
$$
		WITH Talks_Evaluated_By_All AS
		(SELECT te.talk AS talk_id 
		FROM talk_evaluation te
		GROUP BY talk_id
		ORDER BY AVG(te.evaluation) DESC
		)
		SELECT t.talk_id, t.start_time, t.description, t.room
		FROM Talks_Evaluated_By_All JOIN talks t USING(talk_id)
		WHERE t.start_time >= $1 AND t.start_time <= $2
		LIMIT $3;
$$ LANGUAGE SQL;

CREATE FUNCTION consider_initial_evaluation() RETURNS TRIGGER AS
$$
	BEGIN
		INSERT INTO talk_evaluation values (NEW.coordinator, NEW.talk_id, NEW.initial_eval);
		RETURN NEW;
	END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER consider_initial_evaluation AFTER INSERT OR UPDATE ON talks
	FOR EACH ROW EXECUTE PROCEDURE consider_initial_evaluation();

-- (*N) most_popular_talks <start_timestamp> <end_timestamp> <limit> 
-- zwraca referaty rozpoczynające się w podanym przedziału czasowego posortowane malejąco wg obecności, wypisuje 
-- pierwsze <limit> referatów, przy czym 0 oznacza, że należy wypisać wszystkie
-- <talk> <start_timestamp> <title> <room>
CREATE FUNCTION most_popular_talks(TIMESTAMP WITHOUT TIME ZONE, TIMESTAMP WITHOUT TIME ZONE, int)
	RETURNS SETOF talks_type AS
$$
		WITH Talks_Sorted_By_Attendance AS
		(SELECT ta.talk AS talk_id
		FROM talk_attendance ta
		WHERE ta.attendance='present'
		GROUP BY talk_id
		ORDER BY COUNT(*) DESC
		)
		SELECT t.talk_id, t.start_time, t.description, t.room
		FROM Talks_Sorted_By_Attendance JOIN talks t USING(talk_id)
		WHERE t.start_time >= $1 AND t.start_time <= $2
		LIMIT $3;
$$ LANGUAGE SQL;

-- (*U) attended_talks <login> <password> 
-- zwraca dla danego uczestnika referaty, na których był obecny 
-- <talk> <start_timestamp> <title> <room>
CREATE FUNCTION attended_talks(varchar(20))
	RETURNS SETOF talks_type AS
$$
		WITH Talks_Attended_By_User AS
		(SELECT ta.talk AS talk_id
		FROM talk_attendance ta
		WHERE ta.participant=$1 AND ta.attendance='present'
		)
		SELECT t.talk_id, t.start_time, t.description, t.room
		FROM Talks_Attended_By_User JOIN talks t USING(talk_id);
$$ LANGUAGE SQL;

CREATE TYPE abandoned_talks_type AS
	(talk_id varchar(20),
	start_time TIMESTAMP WITHOUT TIME ZONE,
	description varchar(200),
	room smallint,
	num bigint
);

-- (*O) abandoned_talks <login> <password> <limit> 
-- zwraca listę referatów posortowaną malejąco wg liczby uczestników <number> zarejestrowanych na wydarzenie obejmujące referat, 
-- którzy nie byli na tym referacie obecni, wypisuje pierwsze <limit> referatów, przy czym 0 oznacza, że należy wypisać wszystkie
-- <talk> <start_timestamp> <title> <room> <number>


CREATE FUNCTION abandoned_talks(int) RETURNS SETOF abandoned_talks_type AS
$$
		WITH Talks_Sorted_By_Betrayal AS
		(
			WITH Abandoned_Talks AS
			(
				(SELECT t.event, t.talk_id, pr.participant, ta.attendance, ta.participant
				FROM talk_attendance ta 
				RIGHT JOIN talks t ON (ta.talk=t.talk_id)
				LEFT JOIN participant_registrations pr USING(event)
				WHERE pr.participant IS NOT NULL
				)
				EXCEPT
				(SELECT t.event, t.talk_id, pr.participant, ta.attendance, ta.participant
				FROM talk_attendance ta 
				RIGHT JOIN talks t ON (ta.talk=t.talk_id)
				LEFT JOIN participant_registrations pr USING(event)
				WHERE ta.attendance='present' AND pr.participant=ta.participant
				)
			)
			SELECT talk_id, COUNT(*) AS num FROM Abandoned_Talks
			GROUP BY talk_id
			ORDER BY COUNT(*) DESC
		)
		SELECT t.talk_id, t.start_time, t.description, t.room, tsbb.num
		FROM Talks_Sorted_By_Betrayal tsbb JOIN talks t USING(talk_id)
		LIMIT $1;
$$ LANGUAGE SQL;
------------------------------------------------------------------------


