#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <map>

#include "serve_response.h"
#include "server_containers.h"
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
  std::map<std::string, std::string> mime_map;

  void init_acceptor();
  void build_map();
  std::string extension_to_type(std::string ext);
  bool parse_request(char* req_buffer, ParsedRequest* parsed_req); 
 
};


#endif

