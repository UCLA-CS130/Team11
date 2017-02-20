#ifndef HTTP_CONSTANTS_H
#define HTTP_CONSTANTS_H

#include <string>

// CONFIG: 
const int MAX_PORT_NUM = 65535;
const std::string PORT = "port"; 
const std::string PATH = "path";  

// SERVER:
const std::string DEFAULT_REQUEST = "/";
const std::string ECHO_REQUEST = "/echo";

// MIME TYPES:
const std::string GIF = "image/gif";
const std::string HTML = "text/html";
const std::string JPG = "image/jpeg";
const std::string PNG = "image/png";

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

// RESPONSE TEMPLATES:

const std::string DEFAULT_RESPONSE = "<html><h1> Serve is running </h1></html";
const std::string BAD_RESPONSE = "<html><h1> 400 Bad Request </h1></html";
const std::string SERVER_ERROR_RESPONSE = "<html><h1> 500 Internal Server Error </h1></html";
const std::string NOT_FOUND_RESPONSE = "<html><h1> 404 Not Found </h1></html";


#endif