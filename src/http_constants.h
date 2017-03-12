#ifndef HTTP_CONSTANTS_H
#define HTTP_CONSTANTS_H

#include <string>

// CONFIG: 
const int MAX_PORT_NUM = 65535;
const int MAX_LENGTH = 1024;
const std::string PORT = "port"; 
const std::string PATH = "path"; 
const std::string ROOT = "root";
const std::string USER = "user";
const std::string PASS = "pass";
const std::string NUM_THREADS = "num_threads";
const std::string DEFAULT = "default";
const std::string CRLF = "\r\n"; 
const std::string NAME_VALUE_SEPARATOR = ": ";

// SERVER:
const std::string NOT_FOUND_HANDLER = "NotFoundHandler";
const std::string NOT_FOUND_HANDLER_URI = "/NotFoundHandler";


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
const std::string HTTP_OK = "200 OK";
const std::string HTTP_BAD_REQUEST = "400 Bad Request";
const std::string HTTP_NOT_FOUND = "404 Not Found";
const std::string HTTP_INTERNAL_SERVER_ERROR = "500 Internal Server Error";
const std::string HTTP_NOT_IMPLEMENTED = "501 Not Implemented";
const std::string HTTP_FORBIDDEN = "403 Forbidden";

// RESPONSE TEMPLATES:
const std::string DEFAULT_RESPONSE = "<html><h1> Serve is running </h1></html";
const std::string BAD_RESPONSE = "<html><h1> 400 Bad Request </h1></html";
const std::string SERVER_ERROR_RESPONSE = "<html><h1> 500 Internal Server Error </h1></html";
const std::string NOT_FOUND_RESPONSE = "<html><h1> 404 Not Found </h1></html";


#endif