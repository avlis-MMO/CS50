SELECT DISTINCT name FROM people
JOIN stars ON stars.person_id = people.id
WHERE movie_id IN 
(SELECT movie_id FROM Stars
JOIN people ON stars.person_id = people.id
WHERE name = 'Kevin Bacon' AND birth = 1958)
AND name != 'Kevin Bacon'
