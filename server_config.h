#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include "config_parser.h"
#include "request_handler.h"

class ServerConfig 
{
public:
  ServerConfig() : port_(-1) {}
  ~ServerConfig(); 
  bool parse_config(const char* arg); 
  int get_port();
  bool get_handler_config(NginxConfigStatement* statement, NginxConfig* handler_config);  

private: 
  NginxConfig* config_; 
  int port_;
  std::map<std::string, RequestHandler*> handler_map; 
};


#endif