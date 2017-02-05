#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "serve_response.h"

using boost::asio::ip::tcp;

Header make_header(std::string name, std::string value);

class Server
{
public:
  
  Server(unsigned short port);
  void listen();
  ~Server();

private:
  boost::asio::io_service io_service_;
  tcp::acceptor acceptor_;

  void init_acceptor(unsigned short port); 
 
    
};

#endif

