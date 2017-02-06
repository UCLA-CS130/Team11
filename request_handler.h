#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include "server_containers.h"
using boost::asio::ip::tcp;

class RequestHandler {
public:
  RequestHandler() {}
  RequestHandler(ParsedRequest* parsed_req, std::map<std::string, std::string> m)
  : req(parsed_req), uri_path_map(m) {}
  virtual ~RequestHandler();

  bool write_headers(tcp::socket& sock);
  
  virtual bool handle_request() = 0; 
  virtual bool write_body(tcp::socket& sock) = 0;

private:
  ParsedRequest* req;
  std::map<std::string, std::string> uri_path_map; 
}; 

class EchoRequestHandler : public RequestHandler {
public:
  EchoRequestHandler(ParsedRequest* parsed_req, std::map<std::string, std::string> m)
  : RequestHandler(parsed_req, m) {}
  
  ~EchoRequestHandler(); 

  bool handle_request();
  bool write_body(tcp::socket& sock);
};

class StaticRequestHandler : public RequestHandler {
public:
  StaticRequestHandler(ParsedRequest* parsed_req, std::map<std::string, std::string> m)
  : RequestHandler(parsed_req, m) {}
  
  ~StaticRequestHandler(); 

  bool handle_request();
  bool write_body(tcp::socket& sock);
};

#endif