# Team 11 - Assignment 9 

## New Feature: Markdown Handling

## Demo

This file! This file was written in markdown and is being served as an HTML file.
An additional demo file can be found [here](http://localhost:8011/static1/markdown.md)

### Implementation 

* Extended our `StaticHandler` to detect files that feature the `.md` extension. 
* Utilized the [cpp-markdown](https://sourceforge.net/projects/cpp-markdown/) library

## New Feature: Database Interface 

### Demo

We have a main table "movies" that can be queried by name in the following way:

[http://localhost:8011/database](localhost:8011/database)

Displays a full table of all the movies in our database.

[http://localhost:8011/database/Logan](localhost:8011/database/Logan)

Displays year and genre information on the movie Logan.

[http://localhost:8011/database/Lego](localhost:8011/database/Lego)

Although the entire name is not specified, still displays year and genre information on the movie The Lego Batman Movie.

[http://localhost:8011/database/B](localhost:8011/database/B)

Displays information on all movies containing a "B".

### Implementation 
We implemented a simple database interface by defining a new **Database Handler**.
We used the [C++ MySQL Connector API](https://dev.mysql.com/doc/connector-cpp/en/).

By using the free mysql hosting site sql3.freemysqlhosting.net, we have our databases online.
The config specifies the following for establish the database connection:
* host
* database
* username
* password
