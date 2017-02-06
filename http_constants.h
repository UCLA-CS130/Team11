#ifndef HTTP_CONSTANTS_H
#define HTTP_CONSTANTS_H

#include <string>

// HTTP HEADER FIELDS:
const std::string CONTENT_TYPE = "Content-Type";
const std::string SERVER = "Server";
const std::string DATE = "Date"; 

// HTTP STATUS CODES: 
const std::string OK = "200 OK";
const std::string BAD_REQUEST = "400 Bad Request";
const std::string NOT_FOUND = "404 Not Found";
const std::string INTERNAL_SERVER_ERROR = "500 Internal Server Error";
const std::string NOT_IMPLEMENTED = "501 Not Implemented";

#endif