#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <map>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

#include "response.h"
#include "config_parser.h"
#include "request.h"

#include "mysql_connection.h"
#include "mysql_driver.h"


#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


class RequestHandler {
public:
  enum Status {
    OK = 0,
    MISSING_ROOT = 1,
    INVALID_PATH = 2,
    INVALID_RESPONSE = 3,
    FILE_NOT_FOUND = 4,
    DATABASE_ERROR = 5,
    INVALID_CONFIG = 6
    // Define your status codes here.
  };

  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config) = 0;

  virtual Status HandleRequest(const Request& request,
                               Response* response) = 0;

  static RequestHandler* CreateByName(const char* type);
  virtual std::string GetName() = 0;
  std::string uri() { return uri_;}

protected:
  std::string uri_;
  std::string root_;
};

// Registerer code taken from: https://github.com/jfarrell468/registerer
extern std::map<std::string, RequestHandler* (*)(void)>* request_handler_builders;
template<typename T>
class RequestHandlerRegisterer {
 public:
  RequestHandlerRegisterer(const std::string& type) {
    if (request_handler_builders == nullptr) {
      request_handler_builders = new std::map<std::string, RequestHandler* (*)(void)>;
    }
    (*request_handler_builders)[type] = RequestHandlerRegisterer::Create;
  }
  static RequestHandler* Create() {
    return new T;
  }
};

#define REGISTER_REQUEST_HANDLER(ClassName) \
  static RequestHandlerRegisterer<ClassName> ClassName##__registerer(#ClassName)

/** REQUEST HANDLERS */

class EchoHandler : public RequestHandler {
 public:
  virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual Status HandleRequest(const Request& request, Response* response);
  virtual std::string GetName();
};

REGISTER_REQUEST_HANDLER(EchoHandler);

class StaticHandler : public RequestHandler {
 public:
  virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual Status HandleRequest(const Request& request, Response* response);
  virtual std::string GetName();
};

REGISTER_REQUEST_HANDLER(StaticHandler);

class NotFoundHandler: public RequestHandler {
public:
  virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual Status HandleRequest(const Request& request, Response* response);
  virtual std::string GetName();
};

REGISTER_REQUEST_HANDLER(NotFoundHandler);

class StatusHandler : public RequestHandler {
  public:
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
    virtual std::string GetName();
};

REGISTER_REQUEST_HANDLER(StatusHandler);

class ProxyHandler : public RequestHandler {
  public:
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
    virtual std::string GetName();
  private:
    std::string port_ = "80";
};

REGISTER_REQUEST_HANDLER(ProxyHandler);

class DatabaseHandler : public RequestHandler {
  public:
    virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
    virtual Status HandleRequest(const Request& request, Response* response);
    virtual std::string GetName();
    const std::string URLDecode(const std::string& str);
    unsigned char FromHex(unsigned char ch);
  private:
    std::string user_name_ = "";
    std::string password_ = "";
    std::string host_ = "";
    std::string database_ = "";
    sql::mysql::MySQL_Driver *driver_;
    std::string original_uri_ = "";

};

REGISTER_REQUEST_HANDLER(DatabaseHandler);


#endif  // REQUEST_HANDLER_H