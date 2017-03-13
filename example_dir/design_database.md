## Database Interface 

### Demo
We implemented a simple database interface by defining a new **Database Handler**.

By using the free mysql hosting site sql3.freemysqlhosting.net, we have our databases online.
The config specifies the following for establish the database connection:
* host
* database
* username
* password

We have a main table "movies" that can be queried by name in the following way:

`localhost:8011/database`
Displays a full table of all the movies in our database.

`localhost:8011/database/Logan`
Displays year and genre information on the movie Logan.

`localhost:8011/database/Lego Batman`
Although the entire name is not specified, still displays year and genre information on the movie The Lego Batman Movie.

`localhost:8011/database/B`
Displays information on all movies containing a "B".

### Implementation 

A new database handler was created that handles database queries via the `/database` path. 
We used the [C++ MySQL Connector API](https://dev.mysql.com/doc/connector-cpp/en/).