#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include "server_containers.h"

#ifdef TEST_REQUEST_HANDLER
#include "gtest/gtest_prod.h"
#endif

using boost::asio::ip::tcp;

class RequestHandler {
public:
  RequestHandler() {}
  RequestHandler(ParsedRequest* parsed_req, std::map<std::string, std::string> m, Response* server_resp)
  : req(parsed_req), resp(server_resp), uri_path_map(m)  {}
  virtual ~RequestHandler();

  bool write_headers(tcp::socket& sock);
  
  virtual bool handle_request() = 0; 
  virtual bool write_body(tcp::socket& sock) = 0;

protected:
  ParsedRequest* req;
  Response* resp; 
  std::map<std::string, std::string> uri_path_map;
  std::vector<Header> headers;
  std::string name_value_separator = ": ";
  std::string crlf = "\r\n"; 
  boost::filesystem::ifstream* file_stream = nullptr;
  std::string build_headers(); 
  
}; 

class EchoRequestHandler : public RequestHandler {
public:
  EchoRequestHandler(ParsedRequest* parsed_req, std::map<std::string, std::string> m, Response* server_resp)
  : RequestHandler(parsed_req, m, server_resp) {}
  
  ~EchoRequestHandler(); 

  bool handle_request();
  bool write_body(tcp::socket& sock);
private:
  #ifdef TEST_REQUEST_HANDLER
  FRIEND_TEST(HeaderTest, BuildHeader);
  #endif
};

class StaticRequestHandler : public RequestHandler {
public:
  StaticRequestHandler(ParsedRequest* parsed_req, std::map<std::string, std::string> m, Response* server_resp)
  : RequestHandler(parsed_req, m, server_resp) {}
  
  ~StaticRequestHandler(); 

  bool handle_request();
  bool write_body(tcp::socket& sock);
};

#endif