#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include "request_handler.h"

class Response {
  public:
    enum ResponseCode {
      OK = 200,
      INTERNAL_SERVER_ERROR = 500,
      BAD_REQUEST = 400,
      FORBIDDEN = 403,
      NOT_FOUND = 404
    };
  
    Response() : response_body_("") {}
    void SetStatus(const ResponseCode response_code);
    void AddHeader(const std::string& header_name, const std::string& header_value);
    void SetBody(const std::string& body);
    std::string ToString();

    using Headers = std::vector<std::pair<std::string, std::string>>;

  private:
    ResponseCode status_;
    std::string response_body_;
    Headers headers_container_;
    
    //Helper function
    std::string getTextForEnum( int enumVal );
};

#endif