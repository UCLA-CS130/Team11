#ifndef SERVER_CONTAINERS_H
#define SERVER_CONTAINERS_H

#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "http_constants.h"

struct ParsedRequest {
  ParsedRequest(char* req_buffer, size_t req_size) 
  : request(req_buffer), request_size(req_size), method(""), URI(""), HTTP(""), path(""), file(""), mime_type("") {}
  std::string request;
  size_t request_size;  
  std::string method; 
  std::string URI;  
  std::string HTTP; 
  std::string path; 
  std::string file;
  std::string mime_type; 

  void print_contents() {
    std::cout << "ParsedResponse contains:" << std::endl;
    std::cout << "request:\n" << request << std::endl;
    std::cout << "METHOD: " << method << std::endl;
    std::cout << "URI: " << URI << std::endl;
    std::cout << "http: " << HTTP << std::endl;
    std::cout << "path: " << path << std::endl;
    std::cout << "file: " << file << std::endl;
    std::cout << "mime_type: " << mime_type << std::endl;   
  } 
};

struct Header {
  Header(std::string name, std::string value) : name_(name), value_(value) {}
  std::string name_;
  std::string value_;
};


struct Response {
  // Initalize response to be at 500 Internal Server Error
  Response() : status(INTERNAL_SERVER_ERROR), date(""), server("Serve 2.0") {}
  std::string status; 
  std::string date;
  std::string server;
   


};


#endif