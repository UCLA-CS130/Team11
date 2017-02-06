#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include "request_handler.h"
#include "server.h"
#include "server_containers.h"

using boost::asio::ip::tcp;

/* REQUEST HANDLER */
RequestHandler::~RequestHandler() {
  std::cout << "Deconstructing request handler" << std::endl;
}

bool RequestHandler::write_headers(tcp::socket& sock) {
  std::cout << "attempting to write headers" << std::endl;
  return true;
}

/* ECHO REQUEST HANDLER */
EchoRequestHandler::~EchoRequestHandler() {
  std::cout << "Deconstructing echo request handler" << std::endl;
}

bool EchoRequestHandler::handle_request() {
  std::cout << "Echo request - handle request" << std::endl;
  return true;
}

bool EchoRequestHandler::write_body(tcp::socket& sock) {
  std::cout << "Echo request - write body" << std::endl;
  return true;
}

/* STATIC REQUEST HANDLER */
StaticRequestHandler::~StaticRequestHandler() {
  std::cout << "Deconstructing static request handler" << std::endl;
}

bool StaticRequestHandler::handle_request() {
  std::cout << "Static request - handle request" << std::endl;
  return true;
}

bool StaticRequestHandler::write_body(tcp::socket& sock) {
  std::cout << "static request - write body" << std::endl;
  return true;
}

