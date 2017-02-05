#ifndef PARSED_REQUEST_H
#define PARSED_REQUEST_H

#include <string>
#include <iostream>
/**
 * Container for response sent to server
 */
class ParsedRequest {
public:
  ParsedRequest(char* req_buffer) 
  : request(req_buffer), method(""), URI(""), HTTP(""), path(""), file("") {}
  std::string request; 
  std::string method; 
  std::string URI; 
  std::string HTTP; 
  std::string path; 
  std::string file;

  void print_contents() {
    std::cout << "ParsedResponse contains:" << std::endl;
    std::cout << "METHOD: " << method << std::endl;
    std::cout << "URI: " << URI << std::endl;
    std::cout << "http: " << HTTP << std::endl;
    std::cout << "path: " << path << std::endl;
    std::cout << "file: " << file << std::endl;
    std::cout << "request:\n" << request << std::endl;
  } 
};
#endif