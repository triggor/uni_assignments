-- Zadanie 1
-- Igor Tryhub, MABI

SELECT count (*) FROM person WHERE
	gender = 'female' AND
	extract(month FROM birthday)='1' AND
	browserused='Firefox' AND
	extract(year FROM current_date) - extract(year FROM birthday)  < 30
;

-- Zadanie 2
-- Igor Tryhub, MABI

SELECT DISTINCT firstname, lastname FROM 
	person 
	JOIN post_hascreator_person ON (person.id=post_hascreator_person.personid)
	JOIN comment_hascreator_person USING (personid)
	WHERE lastname like 'E%'
	ORDER BY lastname, firstname
;

-- Zadanie 3
-- Igor Tryhub, MABI

SELECT DISTINCT firstname, lastname FROM person JOIN
	(post_hascreator_person 
	JOIN comment_replyof_post USING (postid)	
	JOIN comment_hascreator_person USING (
	)
	ON (person.id=post_hascreator_person.personid)
	WHERE lastname like 'E%' AND
	post_hascreator_person.personid=comment_hascreator_person.personid
	ORDER BY lastname, firstname
;

-- Zadanie 4
-- Igor Tryhub, MABI

SELECT firstname, lastname FROM person EXCEPT()...

