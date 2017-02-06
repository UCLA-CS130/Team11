#ifndef SERVER_CONTAINERS_H
#define SERVER_CONTAINERS_H

#include <string>
#include <iostream>

class ParsedRequest {
public:
  ParsedRequest(char* req_buffer) 
  : request(req_buffer), method(""), URI(""), HTTP(""), path(""), file(""), mime_type("") {}
  std::string request; 
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

#endif