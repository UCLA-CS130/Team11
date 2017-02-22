#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <vector>


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
    int GetStatus();
    std::string ToString();

  private:
    ResponseCode status_;
    std::string response_body_;
    std::vector<std::pair<std::string, std::string>> headers_container_;
    
    //Helper function
    std::string getTextForEnum( int enumVal );
};

#endif