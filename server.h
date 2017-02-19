#ifndef SERVER_H
#define SERVER_H


#include <boost/asio.hpp>
#include <iostream>
#include "request_handler.h"

class Server {
public: 
  Server(); 
  ~Server(); 
  bool init(const char* config_file);
  RequestHandler* get_handler(std::string);  

private: 
  std::map<std::string, RequestHandler*> handler_map; 
};

#endif