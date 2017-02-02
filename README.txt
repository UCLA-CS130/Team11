# Get Served

**Serve** is web server built with C++. 

## Setup
Runs in Ubuntu 14.04. 

Install the following dependencies: 

**Boost**

`sudo apt-get install libboost-all-dev`

Run `make` to build the server.

To run the server, run: `./server config_file`

The config file is required to pass the port to the server. Config files should be in the following format:

```
listen [port #];
```

Send requests to Serve via `localhost:[port #]`