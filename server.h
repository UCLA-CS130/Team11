#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "serve_response.h"
#include "parsed_request.h"
#include "server_config.h"

using boost::asio::ip::tcp;

Header make_header(std::string name, std::string value);

class Server
{
public:
  
  Server();
  ~Server();
  void listen();
  bool init(const char* config_file);
  
private:
  boost::asio::io_service io_service_;
  tcp::acceptor acceptor_;
  ServerConfig* server_config; 

  void init_acceptor();
  bool parse_request(char* req_buffer, ParsedRequest* parsed_req); 
 
};

#endif

