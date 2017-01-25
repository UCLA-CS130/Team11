#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server
{
public:
	
  Server(unsigned short port);
  ~Server();

private:
  boost::asio::io_service io_service_;
  tcp::acceptor acceptor_;
  void serve();
    
};

#endif