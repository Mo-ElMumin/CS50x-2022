select
    avg(rating)
    from ratings
    join movies on movies.id = ratings.movie_id
    where movies.year = 2012