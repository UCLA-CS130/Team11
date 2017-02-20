#ifndef SERVER_H
#define SERVER_H


#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include "server_config.h"

using boost::asio::ip::tcp;

class Server {
public: 
  Server(); 
  ~Server(); 
  bool init(const char* config_file);

private: 
  boost::asio::io_service io_service_;
  tcp::acceptor acceptor_;
  ServerConfig* server_config; 

  void init_acceptor();
};

#endif