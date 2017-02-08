#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <map>

#include "server_containers.h"
#include "server_config.h"

// USED BY TESTING CLASS
#ifdef TEST_SERVER_CONFIG
#include "gtest/gtest_prod.h"
#endif

using boost::asio::ip::tcp;

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

  #ifdef TEST_SERVER
  FRIEND_TEST(ServerInitTest, NullServerConfig);
  FRIEND_TEST(ServerInitTest, ServerConfigError);
  FRIEND_TEST(ServerInitTest, ValidServerConfig);
  FRIEND_TEST(ServerExtensionToTypeTest, ReturnValidTypeTest);
  FRIEND_TEST(ServerExtensionToTypeTest, ReturnDefaultTypeTest);
  FRIEND_TEST(ParseRequestTest, InvalidRequest);
  FRIEND_TEST(ParseRequestTest, ValidRequest);
  FRIEND_TEST(ParseRequestTest, ValidFileRequest);
  #endif
  
 
};


#endif

