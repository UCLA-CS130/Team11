#include "response.h"
#include "http_constants.h"

void Response::SetStatus(const ResponseCode response_code)
{
  status_ = response_code;
}


void Response::SetBody(const std::string& body)
{
  response_body_ = body;
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value)
{
  std::pair<std::string, std::string> header(header_name, header_value);
  headers_container_.push_back(header);
}

std::string Response::ToString()
{
  std::string response_header = "";
  //fix status portion
  response_header += "HTTP/1.0 " +  getTextForEnum(status_) + CRLF;

  for (unsigned int i = 0; i < headers_container_.size(); ++i)
  {
    response_header += headers_container_[i].first;
    response_header += NAME_VALUE_SEPARATOR;
    response_header += headers_container_[i].second;
    response_header += CRLF;
  }

  response_header += CRLF;
  response_header += response_body_;

  return response_header; 
}

//helper function
std::string Response::getTextForEnum( int enumVal )
{
  switch( enumVal )
  {
    case ResponseCode::OK:
      return "200 OK";
    case ResponseCode::INTERNAL_SERVER_ERROR:
      return "500 INTERNAL SERVER ERROR";
    case ResponseCode::NOT_FOUND:
      return "404 NOT FOUND";
    case ResponseCode::FORBIDDEN:
      return "403 FORBIDDEN";
    case ResponseCode::BAD_REQUEST:
      return "400 BAD REQUEST";

    default:
      return "NOT RECOGNIZED";
  }
}
  