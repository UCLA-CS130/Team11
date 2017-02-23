# Serve 💌
A C++ synchronous webserver powered by Boost 

## Team 11
* Breanna Nery
* Natalie Ethell
* Stephen Link

## Contributing 
### Installation
```
git clone --recursive https://github.com/UCLA-CS130/Team11.git
```
### Dependencies
* Designed to run best in Ubuntu 14.04
* Boost
  * `sudo apt-get install libboost-all-dev`
* Python
  * `integration_test.py` is written for Python 2.7
* [Requests](http://docs.python-requests.org/en/master/) module 
  * Required to run `integration_test.py`
  * `pip install requests`      

### Build

* Run `make` to build Serve
* Run `./serve [config_file]` to execute Serve. 
  * `new_config` in our main directory adheres to the config file format specified in the common api 

### Test
* Don't forget to install the requests module! See the dependencies section above
* Run `make` test. This will execute our unit tests and our integration test 

### Code Layout
* `src`: Contains all our source and header files neccessary to build the server
  * `serve_main.cc`: Instatiates the `Server` class and initalizes the server. Afterwards, it calls the server's `listen()` function
  * `server.cc`: Defines the server's `init()` and `listen()` function. Requests are read in and responses are written out in `listen()`
  * `server_config.cc`: Responsible for parsing the config file and creating the RequestHandlers and the map. Also used in routing requests to the correct handler. 
  * `request.cc`: The high level representation of the request adhering to the common api 
  * `response.cc`: The high level representation of the response adhering to the common api.
  * `request_handlers.cc`: This is where the request handler base class and all derived request handlers are defined. 
  * `http_constants.h` is our main file for defining all constants used by Serve
* `test`: Contains all our unit test files and our integration test
 
### Code Style
* Ensure tabs are represented as spaces and a tab width of 2 is used
* Member variables are defined as `[variable_name]_`
* We use Boost Trivial Logging for general debug statements and warning statements. [Here](http://www.boost.org/doc/libs/1_62_0/libs/log/doc/html/log/tutorial.html) is an example of how Boost logging is used.
* Only place library includes in the header file. The source file should only contain includes to its respective header file and `http_constants.h` 