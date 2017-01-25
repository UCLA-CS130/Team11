#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "serve_response.h"


std::string Response::response_builder(std::string status, std::string content)
{
  std::string response = "";
  response += status + crlf;
  for (int i = 0; i < headers.size(); ++i)
  {
    Header& h = headers[i];
    response += h.name;
    response += name_value_separator;
    response += h.value;
    response += crlf;
  }
  response += crlf;
  response += content;
  return response;
}