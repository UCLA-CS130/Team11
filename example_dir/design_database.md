##Database Interface##

We implemented a simple database interface by defining a new ***Database Handler.***

By using the free mysql hosting site sql3.freemysqlhosting.net, we have our databases online.
The config specifies the username and password for us to connect to our database.

We have a main table "movies" that can be queried by name in the following way:

`localhost:9999/database`
Displays a full table of all the elements.

`localhost:9999/database/Logan`
Displays year and genre information on the movie Logan.

`localhost:9999/database/Lego Batman`
Although the entire name is not specified, still displays year and genre information on the movie The Lego Batman Movie.

`localhost:9999/database/B`
Displays information on all movies containing a "B".