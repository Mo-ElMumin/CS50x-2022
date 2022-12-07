select
    people.name
    from people
    join stars on people.id = stars.person_id
    join movies on stars.movie_id = movies.id
    where movies.id
        in (select movies.id
            from people
            join stars on people.id = stars.person_id
            join movies on stars.movie_id = movies.id
            where people.name = "Kevin Bacon" and people.birth = 1958)
    and people.name != "Kevin Bacon"
