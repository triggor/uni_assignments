/*======Projekt: System wspomagający organizację konferencji======*/
/*==========Model fizyczny. Wykonał: Igor Tryhub, 275235==========*/


CREATE ROLE admin SUPERUSER CREATEDB CREATEROLE REPLICATION PASSWORD 'admin';
CREATE DATABASE dbproj WITH OWNER admin TEMPLATE template0;
\connect dbproj
CREATE SCHEMA projschema AUTHORIZATION admin;
SET search_path TO projschema;
GRANT ALL PRIVILEGES ON DATABASE dbproj TO admin;
CREATE USER organizer LOGIN PASSWORD 'organizer';
CREATE USER participant LOGIN;
CREATE TYPE rolelist AS ENUM ('organizer', 'participant');
CREATE TYPE statuslist AS ENUM ('proposed', 'rejected', 'confirmed');
CREATE TYPE attendancelist AS ENUM ('present', 'absent');

------------------------------------------------------------------------
CREATE TABLE users (
	login varchar(20) PRIMARY KEY CHECK (login <> '' AND login ~ '^[a-zA-Z0-9_-]*$'),
	password varchar(20) NOT NULL CHECK (char_length(password) >= 4),
	role rolelist NOT NULL DEFAULT 'participant'
);
ALTER TABLE users OWNER TO admin;

CREATE TABLE events (
	title varchar(50) PRIMARY KEY CHECK (char_length(title) >= 4),
	creator varchar(20) NOT NULL REFERENCES users(login),
	start_time TIMESTAMP WITHOUT TIME ZONE NOT NULL,
	end_time TIMESTAMP WITHOUT TIME ZONE NOT NULL,
	CHECK (start_time < end_time)
);
ALTER TABLE events OWNER TO admin;

CREATE VIEW event_duration_view AS
	(SELECT events.title, events.end_time-events.start_time AS duration
	FROM events);
ALTER VIEW event_duration_view OWNER TO admin;

CREATE TABLE rooms (
	room_no smallint PRIMARY KEY,
	capacity smallint NOT NULL DEFAULT 50,
	CHECK (room_no >= 0 AND capacity >= 0)
);
ALTER TABLE rooms OWNER TO admin;

CREATE TABLE proposed_talks (
	talk_id SERIAL PRIMARY KEY,
	event varchar(50) NULL REFERENCES events(title),
	speaker varchar(20) NOT NULL REFERENCES users(login),
	description varchar(200) NOT NULL,
	status statuslist NOT NULL DEFAULT 'proposed',
	evaluator varchar(20) NULL REFERENCES users(login),
	initial_eval smallint NULL,
	room smallint REFERENCES rooms(room_no) NULL,
	CHECK (initial_eval >= 0 AND initial_eval <= 10 AND char_length(description) >= 30)
);
ALTER TABLE proposed_talks OWNER TO admin;

CREATE VIEW talks_view AS
	(SELECT pt.talk_id, pt.event, pt.speaker, pt.description, pt.room
	FROM proposed_talks pt
	WHERE status = 'confirmed');
ALTER VIEW talks_view OWNER TO admin;

CREATE TABLE friendship_requests (
	participant varchar(20) NOT NULL REFERENCES users(login),
	friends_with varchar(20) NOT NULL REFERENCES users(login),
	PRIMARY KEY (participant, friends_with),
	CHECK (participant <> friends_with)
);
ALTER TABLE friendship_requests OWNER TO admin;

CREATE VIEW friends_view AS
	(SELECT fr1.participant AS friend1, fr2.participant AS friend2
	FROM friendship_requests fr1
	JOIN friendship_requests fr2 ON (fr1.friends_with=fr2.participant)
	WHERE fr1.participant=fr2.friends_with AND fr1.participant<fr2.participant);
ALTER VIEW friends_view OWNER TO admin;

CREATE TABLE participant_registrations (
	participant varchar(20) NOT NULL REFERENCES users(login),
	event varchar(50) NOT NULL REFERENCES events(title),
	reg_time TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT NOW(),
	PRIMARY KEY (participant, event)
);
ALTER TABLE participant_registrations OWNER TO admin;

CREATE TABLE talk_attendance (
	participant varchar(20) NOT NULL REFERENCES users(login),
	talk bigint NOT NULL REFERENCES proposed_talks(talk_id),
	attendance attendancelist NOT NULL DEFAULT 'absent',
	PRIMARY KEY (participant, talk)
);
ALTER TABLE talk_attendance OWNER TO admin;

CREATE TABLE talk_evaluation (
	participant varchar(20) NOT NULL REFERENCES users(login),
	talk bigint NOT NULL REFERENCES proposed_talks(talk_id),
	evaluation smallint NOT NULL,
	PRIMARY KEY (participant, talk),
	CHECK (evaluation >= 0 AND evaluation <= 10)
);
ALTER TABLE talk_evaluation OWNER TO admin;

CREATE VIEW talk_rating_view AS
	(SELECT te.talk, AVG(te.evaluation) AS rating, COUNT(DISTINCT te.participant) AS votes
	FROM talk_evaluation te
	GROUP BY talk
	ORDER BY rating DESC);
ALTER VIEW talk_rating_view OWNER TO admin;

------------------------------------------------------------------------
INSERT INTO users
	VALUES ('user1', 'pass', 'participant');
INSERT INTO users
	VALUES ('user2', 'password', 'participant');
INSERT INTO users
	VALUES ('organ1', 'passw2', 'organizer');
INSERT INTO events 
	VALUES ('event title 1!', 'organ1', '2017-05-30 10:26:44', '2017-05-30 11:30:00');
INSERT INTO rooms
	VALUES (23);
INSERT INTO proposed_talks
	VALUES(DEFAULT, 'event title 1!', 'user1', 'This is a short description of the proposed talk. It helps the evaluator to evaluate it.');
INSERT INTO proposed_talks
	VALUES(DEFAULT, 'event title 1!', 'user1', 'This is a short description of the proposed talk.');
UPDATE proposed_talks
	SET status='confirmed', evaluator='organ1', initial_eval=8, room=23
	WHERE talk_id=2;
INSERT INTO friendship_requests
	VALUES ('user2', 'user1');
INSERT INTO friendship_requests
	VALUES ('user2', 'user3');
INSERT INTO friendship_requests
	VALUES ('user3', 'user2');
INSERT INTO participant_registrations
	VALUES ('user2', 'event title 1!');
INSERT INTO talk_attendance
	VALUES ('user2', 2);
INSERT INTO talk_evaluation
	VALUES('user2', 2, 9);
INSERT INTO talk_evaluation
	VALUES('user1', 2, 6);
INSERT INTO talk_evaluation
	VALUES('user1', 1, 5);

------------------------------------------------------------------------
/*--list of databases*/
\l
/*--list of roles*/
\du 
/*--list of schemas*/
\dn 
/*--list of relations*/
\d

