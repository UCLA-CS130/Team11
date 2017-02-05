#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "serve_response.h"
#include "parsed_request.h"

using boost::asio::ip::tcp;

Header make_header(std::string name, std::string value);

class Server
{
public:
  
  Server();
  void listen();
  bool init(const char* config_file);
  ~Server();

private:
  boost::asio::io_service io_service_;
  tcp::acceptor acceptor_;
  unsigned short port; 

  void init_acceptor();
  bool parse_request(char* req_buffer, ParsedRequest* parsed_req); 
 
    
};

#endif

