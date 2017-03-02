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

void Response::ClearHeaders() {
  headers_container_.erase(headers_container_.begin(), headers_container_.end());
}

std::string Response::ToString()
{
  if (response_msg_ != ""){
    return response_msg_;
  }

  std::string response_header = "";
  response_header += "HTTP/1.1 " + getTextForEnum(status_) + CRLF;
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

// returns status for status handler
int Response::GetStatus()
{
  switch(status_)
  {
    case ResponseCode::OK:
      return 200;
    case ResponseCode::INTERNAL_SERVER_ERROR:
      return 500;
    case ResponseCode::BAD_REQUEST:
      return 400;
    case ResponseCode::FORBIDDEN:
      return 403;
    case ResponseCode::NOT_FOUND:
      return 404;

    default:
      return -1;
  }
}

//helper function
std::string Response::getTextForEnum( int enumVal )
{
  switch( enumVal )
  {
    case ResponseCode::OK:
      return HTTP_OK;
    case ResponseCode::INTERNAL_SERVER_ERROR:
      return HTTP_INTERNAL_SERVER_ERROR;
    case ResponseCode::NOT_FOUND:
      return HTTP_NOT_FOUND;
    case ResponseCode::FORBIDDEN:
      return HTTP_FORBIDDEN;
    case ResponseCode::BAD_REQUEST:
      return HTTP_BAD_REQUEST;

    default:
      return "NOT RECOGNIZED";
  }
}
  