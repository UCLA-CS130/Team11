#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/filesystem.hpp>

#include <iostream>
#include <string>
#include <vector>
#include "server.h"
#include "serve_response.h"
#include "server_containers.h"
#include "request_handler.h"
#include "server_config.h"

using boost::asio::ip::tcp;

const int MAX_LENGTH = 1024;

void Server::init_acceptor() {
  // Setup for accepting connection, taken from Boost sample documentation
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), server_config->port_num);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
}

void Server::build_map() { 
  mime_map[".gif"] = "image/gif";
  mime_map[".htm"] = "text/html";
  mime_map[".html"] = "text/html";
  mime_map[".jpg"] = "image/jpeg";
  mime_map[".png"] = "image/png";
}

bool Server::init(const char* config_file) {
  server_config = new ServerConfig();

  if ( (server_config == nullptr) || (!server_config->parse_config(config_file)) ) {
    std::cerr << "Error with server's configuration file" << std::endl;
    return false;
  }

  // DEBUGGING 
  std::cout << "Contents of uri_map: " << std::endl;
  for (const auto &p : server_config->uri_map) {
    std::cout << "uri_map[" << p.first << "] = " << p.second << '\n';
  }
  build_map();
  init_acceptor(); 
  return true; 
}

Server::Server() : acceptor_(io_service_), server_config(nullptr) {}

Header make_header(std::string name, std::string value) {
  Header h; 
  h.name = name; 
  h.value = value;
  return h;
}

Server::~Server() {
 delete server_config;
}

std::string Server::extension_to_type(std::string ext) {
  auto it = mime_map.find(ext); 
  if (it != mime_map.end()) {
    return it->second;
  }
  return "text/plain";
}

bool Server::parse_request(char* req_buffer, ParsedRequest* parsed_req) {
  
  // Extract request into lines: 
  std::string request(req_buffer); 
  std::vector<std::string> lines;
  boost::split(lines,request,boost::is_any_of("\r\n"));

  // Extract request line: 
  std::vector<std::string> reqs; 
  boost::split(reqs,lines[0],boost::is_any_of(" "));

  if (reqs.size() != 3) {
    std::cerr << "Malformed request." << std::endl;
    return false;
  }

  parsed_req->method = reqs[0];
  parsed_req->URI = reqs[1];
  parsed_req->HTTP = reqs[2];

  // Extract file and path:
  boost::filesystem::path p(parsed_req->URI);
  parsed_req->path = p.parent_path().string(); 
  parsed_req->file = p.filename().string();
  std::string ext = p.extension().string();
  parsed_req->mime_type = extension_to_type(ext); 
  return true; 
}

void Server::listen(){

  try {
    while(true) {
      // [1] Accept connection:
      tcp::socket socket(io_service_);
      acceptor_.accept(socket);

      // [2] Perform read:
      char req_buffer[MAX_LENGTH];
      boost::system::error_code err;
      size_t num_bytes = socket.read_some(boost::asio::buffer(req_buffer), err);
      if (err == boost::asio::error::eof)
        break; 
      else if (err)
        throw boost::system::system_error(err); 

      ParsedRequest parsed_req(req_buffer); 
      if (parse_request(req_buffer, &parsed_req) == false) {
        std::cerr << "Unable to parse request:\n" << req_buffer << std::endl;
        continue;
      }

      // DEBUGGING: 
      parsed_req.print_contents();

      // TESTING REQUEST HANDLER: 
      // TODO: HANDLE REQUESTS
      EchoRequestHandler echo_request(&parsed_req, server_config->uri_map); 
      StaticRequestHandler static_request(&parsed_req, server_config->uri_map); 

      echo_request.write_headers(socket);
      static_request.write_headers(socket);

      echo_request.write_body(socket);
      static_request.write_body(socket);

      echo_request.handle_request();
      static_request.handle_request();

      // [3] Perform write: Creates a response object that builds the request response
      Response resp;
      resp.headers.push_back(make_header("Content-Type","text/plain")); 
      std::string header = resp.response_builder("HTTP/1.1 200 OK");

      std::cout << "// RESPONSE SENT //" << std::endl;

      boost::asio::write(socket, boost::asio::buffer(header, header.size()));

      boost::asio::write(socket, boost::asio::buffer(req_buffer, num_bytes));
      
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  
}

