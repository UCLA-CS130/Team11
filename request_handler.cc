#include "request_handler.h"

std::map<std::string, RequestHandler* (*)(void)>* request_handler_builders = nullptr;

RequestHandler* RequestHandler::CreateByName(const char* type) {
  const auto type_and_builder = request_handler_builders->find(type);
  if (type_and_builder == request_handler_builders->end()) {
    return nullptr;
  }
  return (*type_and_builder->second)();
}

/** ECHO HANDLER **/
void EchoHandler::HandleRequest(void) {
  std::cout << "EchoHandler::HandleRequest called" << std::endl;
}

/** STATIC FILE HANDLER */
void StaticFileHandler::HandleRequest(void) {
  std::cout << "StaticFileHandler::HandleRequest called" << std::endl;
}