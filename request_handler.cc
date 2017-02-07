#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include <string>

#include "request_handler.h"
#include "server_containers.h"
#include "http_constants.h"

using boost::asio::ip::tcp;

/* REQUEST HANDLER */
RequestHandler::~RequestHandler() {
  std::cout << "Deconstructing request handler" << std::endl;
  delete file_stream;
}

std::string RequestHandler::build_headers() {
  std::string response_header = "";
  response_header += req->HTTP + " " + resp->status + crlf;
  for (unsigned int i = 0; i < headers.size(); ++i)
  {
    Header& h = headers[i];
    response_header += h.name_;
    response_header += name_value_separator;
    response_header += h.value_;
    response_header += crlf;
  }
  response_header += crlf;
  return response_header; 
}

bool RequestHandler::write_headers(tcp::socket& sock) {
  std::string header = build_headers();
  boost::asio::write(sock, boost::asio::buffer(header, header.size()));
  return true;
}

/* ECHO REQUEST HANDLER */
EchoRequestHandler::~EchoRequestHandler() {
  std::cout << "Deconstructing echo request handler" << std::endl;
}

bool EchoRequestHandler::handle_request() {
  std::cout << "Echo request - handle request" << std::endl;
  resp->status = OK;
   // [TODO] Set date 
  headers.push_back(Header(SERVER, resp->server));

  if (req->URI == ECHO_REQUEST) { 
    headers.push_back(Header(CONTENT_TYPE, req->mime_type));  
  }
  else if (req->URI == DEFAULT_REQUEST) {
    headers.push_back(Header(CONTENT_TYPE, HTML));
  }
  else {
    resp->status = BAD_REQUEST;
    headers.push_back(Header(CONTENT_TYPE, HTML)); 
  }
   
  return true;
}

bool EchoRequestHandler::write_body(tcp::socket& sock) {
  std::cout << "Echo request - write body" << std::endl;
  if (req->URI == ECHO_REQUEST) {
    boost::asio::write(sock, boost::asio::buffer(req->request, req->request_size));
  }
  else if (req->URI == DEFAULT_REQUEST) {
    boost::asio::write(sock, boost::asio::buffer(DEFAULT_RESPONSE, DEFAULT_RESPONSE.size()));
  }
  else {
    boost::asio::write(sock, boost::asio::buffer(BAD_RESPONSE, BAD_RESPONSE.size()));
  } 
  return true;
}

/* STATIC REQUEST HANDLER */
StaticRequestHandler::~StaticRequestHandler() {
  std::cout << "Deconstructing static request handler" << std::endl;
}

// [TODO] Look into error handling: 
bool StaticRequestHandler::handle_request() {
  std::cout << "Static request - handle request" << std::endl;
  headers.push_back(Header(SERVER, resp->server));

  std::string file_path = "";
  // Append mapped file path based on URI
  auto it = uri_path_map.find(req->path);
  if (it != uri_path_map.end()) {
    // What if file path already has '/': ie /foo/bar/?
    file_path = it->second + "/" + req->file; 
  }
  else {
    file_path = req->file; 
  }

  // DEBUG: 
  std::cout << "File path to be opened: " << file_path << std::endl;

  // Verify if file exists, set status
  boost::filesystem::path p(file_path);
  if (boost::filesystem::exists(p)) {
    if (boost::filesystem::is_directory(p)) {
      std::cerr << p << " is a directory." << std::endl;
      resp->status = BAD_REQUEST; 
      return false;
    }

    // The file exists, attempt to open file
    std::cout << "Opening file: " << p << std::endl;
    file_stream = new boost::filesystem::ifstream(p);
    if (file_stream == nullptr) {
      resp->status = INTERNAL_SERVER_ERROR;
      headers.push_back(Header(CONTENT_TYPE, HTML)); 
      std::cerr << "Unable to open file" << std::endl;
      return false; 
    }

    if(file_stream->is_open()) {
      std::cout << "File is opened!" << std::endl;
      headers.push_back(Header(CONTENT_TYPE, req->mime_type)); 
      resp->status = OK;
    }
    else {
      resp->status = INTERNAL_SERVER_ERROR;
      headers.push_back(Header(CONTENT_TYPE, HTML)); 
      std::cerr << "Unable to open file" << std::endl;
      return false;
    }
    
  }
  else {
    std::cerr << "File does not exist, sending 404 response" << std::endl;
    resp->status = NOT_FOUND;
    return false; 
  }
  return true;
}

bool StaticRequestHandler::write_body(tcp::socket& sock) {
  std::cout << "static request - write body" << std::endl;
  char buffer[512];
  std::string body = "";

  if (resp->status == BAD_REQUEST) {
    boost::asio::write(sock, boost::asio::buffer(BAD_RESPONSE, BAD_RESPONSE.size()));
  }
  else if (resp->status == INTERNAL_SERVER_ERROR) {
    boost::asio::write(sock, boost::asio::buffer(SERVER_ERROR_RESPONSE, SERVER_ERROR_RESPONSE.size()));
  }
  else if (resp->status == NOT_FOUND) {
    boost::asio::write(sock, boost::asio::buffer(NOT_FOUND_RESPONSE, NOT_FOUND_RESPONSE.size()));
  }
  else { // Attempt to write file
    std::cout << "Attempting to write file" << std::endl;
    while(file_stream->read(buffer, sizeof(buffer)).gcount() > 0) {
      body.append(buffer, file_stream->gcount());
    }

    boost::asio::write(sock, boost::asio::buffer(body.c_str(), body.size()));


  }

  return true;
}

