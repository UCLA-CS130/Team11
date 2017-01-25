#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "serve_response.h"


std::string Response::header_builder()
{
  std::string response = "";
  for (std::size_t i = 0; i < headers.size(); ++i)
  {
    Header& h = Response::headers[i];
    response += h.name;
    response += name_value_separator;
    response += h.value;
    response += crlf;
  }
  response += crlf;
  return response;
}