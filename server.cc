#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "server.h"
#include "serve_response.h"
#include "config_parser.h"
#include "parsed_request.h"



using boost::asio::ip::tcp;

const int MAX_LENGTH = 1024;

void Server::init_acceptor() {
  // Setup for accepting connection, taken from Boost sample documentation
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
}

bool Server::init(const char* config_file) {
  NginxConfig config;
  NginxConfigParser parser;
  if (parser.Parse(config_file, &config) == false) {
    return false; 
  }
  std::string p = config.statements_[0]->tokens_[1];
  port = (short)stoi(p); 
  init_acceptor(); 
  return true; 
}

Server::Server() : acceptor_(io_service_) {}

Header make_header(std::string name, std::string value) {
  Header h; 
  h.name = name; 
  h.value = value;
  return h;
}

Server::~Server() {
  // TODO: Setup deconstructor
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
  std::vector<std::string> paths; 
  boost::split(paths,parsed_req->URI,boost::is_any_of("/"));
  // Ignore paths[0] as empty string is tokenized
  int paths_size = paths.size() - 1; 
  switch(paths_size) {
    case 0:
      std::cerr << "No resource requested" << std::endl;
      // TODO: Set error status
      break;
    case 1:
      parsed_req->path = paths[1];
      break;
    case 2:
      parsed_req->path = paths[1];
      parsed_req->file = paths[2];
      break;
    default:
      std::cerr << "Resource requested not valid" << std::endl;
      // TODO: Set error status
  }
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
      socket.read_some(boost::asio::buffer(req_buffer), err);
      if (err == boost::asio::error::eof)
        break; 
      else if (err)
        throw boost::system::system_error(err); 

      ParsedRequest parsed_req(req_buffer); 
      if (parse_request(req_buffer, &parsed_req) == false) {
        std::cerr << "Unable to parse request." << std::endl;
        continue;
      }

      // DEBUGGING: 
      parsed_req.print_contents();

      // [3] Perform write: Creates a response object that builds the request response
      Response resp;
      resp.headers.push_back(make_header("Content-Type","text/plain")); 
      std::string body(req_buffer);
      std::string resp_headers = resp.response_builder("HTTP/1.1 200 OK", body);
      std::size_t resp_len = resp_headers.size();

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

