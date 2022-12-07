select
    songs.name
from
    songs
    join artists on songs.artist_id = artists.id
    where artists.name = "Post Malone"