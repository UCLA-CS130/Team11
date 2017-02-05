#include "server.h"
#include "serve_response.h"

#include <iostream>
#include <string>

using boost::asio::ip::tcp;

// Read in the request from the socket used to construct the handler, then
// fill the response buffer with the necessary headers followed by the
// original request.
std::string Response::response_builder(std::string status) {

  std::string response_header = "";
  response_header += status + crlf;
  for (unsigned int i = 0; i < headers.size(); ++i)
  {
    Header& h = headers[i];
    response_header += h.name;
    response_header += name_value_separator;
    response_header += h.value;
    response_header += crlf;
  }
  response_header += crlf;
  
  //std::size_t header_size = response_header.size();
  return response_header;
}

