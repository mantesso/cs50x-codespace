SELECT DISTINCT movies.title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Bradley Cooper"
INTERSECT
SELECT DISTINCT movies.title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Jennifer Lawrence";
