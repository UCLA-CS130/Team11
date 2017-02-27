#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <vector>

#include "server_config.h"
#include "request.h"
#include "response.h"
#include "request_handler.h"
#include "status_count.h"
#include <boost/thread.hpp>
using boost::asio::ip::tcp;


class Server {
public: 
  Server(); 
  ~Server(); 
  bool init(const char* config_file);
  void listen(); 
  void spawn_threads();

private: 
  // number of threads
  int num_threads_;
  
  boost::asio::io_service io_service_;

  // for listening for connections
  tcp::acceptor acceptor_;

  // thread group for thread pool
  boost::thread_group threadpool;
  
  ServerConfig* server_config_; 

  void init_acceptor();
};

#endif