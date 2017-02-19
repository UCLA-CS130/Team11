#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <map>
#include <iostream>
#include <memory>
#include <string>

class RequestHandler {
 public:
  enum Status {
    OK = 0
    // Define your status codes here.
  };

  // Temporary handle request for testing register
  virtual void HandleRequest(void) = 0;
  static RequestHandler* CreateByName(const char* type);
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
  virtual void HandleRequest(void);
};

REGISTER_REQUEST_HANDLER(EchoHandler);

class StaticFileHandler : public RequestHandler {
 public:
  virtual void HandleRequest(void);
};

REGISTER_REQUEST_HANDLER(StaticFileHandler);

#endif  // REQUEST_HANDLER_H