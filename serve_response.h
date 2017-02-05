#ifndef RESPONSE_H
#define RESPONSE_H

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <map>

struct Header {
  std::string name;
  std::string value;
};

struct Response {

  std::string name_value_separator = ": ";
  std::string crlf = "\r\n";
  std::vector<Header> headers;
  std::string response_builder(std::string status);

};

#endif

