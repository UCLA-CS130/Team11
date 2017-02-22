#include "request_handler.h"
#include "http_constants.h"

std::map<std::string, RequestHandler* (*)(void)>* request_handler_builders = nullptr;

RequestHandler* RequestHandler::CreateByName(const char* type) {
  const auto type_and_builder = request_handler_builders->find(type);
  if (type_and_builder == request_handler_builders->end()) {
    return nullptr;
  }
  return (*type_and_builder->second)();
}

/** ECHO HANDLER **/
RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
  uri_ = uri_prefix; 
  return OK;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request, Response* response) {
  if (response == nullptr) {
    return INVALID_RESPONSE;
  }
  response->SetStatus(Response::OK); 
  response->AddHeader(CONTENT_TYPE, request.mime_type()); 
  response->SetBody(request.raw_request());
  return OK; 
}

/** STATIC FILE HANDLER */
RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
  uri_ = uri_prefix; 
  for (int i = 0; i < config.statements_.size(); i++) {
    std::vector<std::string> token_list = config.statements_[i]->tokens_; 
    if (token_list.size() < 2) {
      BOOST_LOG_TRIVIAL(warning) << token_list[0] << " missing value. Ignoring statement"; 
      continue;
    }

    std::string token = token_list[0]; 
    std::string value = token_list[1];

    if (token == ROOT) {
      boost::filesystem::path p(value);
      if (boost::filesystem::exists(p) && boost::filesystem::is_directory(p)) {
        root_ = value;
        return OK; 
      }
      else {
        BOOST_LOG_TRIVIAL(warning) << value << " is an invalid path or not a directory."; 
        return INVALID_PATH; 
      }
    }
  }
  return MISSING_ROOT;
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response) {
  if (response == nullptr) {
    return INVALID_RESPONSE;
  }

  response->SetStatus(Response::OK); 
  response->AddHeader(CONTENT_TYPE, request.mime_type());

  std::string file_path = root_ + "/" + request.file(); 

  BOOST_LOG_TRIVIAL(debug) << "File path to be opened: " << file_path << std::endl;

  // Verify if file exists:
  boost::filesystem::path p(file_path);
  if (boost::filesystem::exists(p) && !boost::filesystem::is_directory(p)) {
    // Attempt to open file:
    boost::filesystem::ifstream* file_stream = new boost::filesystem::ifstream(p);
    if (file_stream == nullptr || !file_stream->is_open()) {
      // TODO: For now it will be handled by 404, by this is better as a 500 Internal Service Error
      BOOST_LOG_TRIVIAL(warning) << "The file at " << file_path << "does not exist or is unabled to be opened"; 
      return FILE_NOT_FOUND; 
    }

    // Attempt to read in file and write to body string
    char buffer[512]; 
    std::string body = "";
    while(file_stream->read(buffer, sizeof(buffer)).gcount() > 0) {
      body.append(buffer, file_stream->gcount());
    }

    response->SetBody(body); 
    //TODO: Potentially have some checks here? 
  }
  else {
    return FILE_NOT_FOUND; 
  }

  return OK; 
}

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
  //uri_ = uri_prefix; 
  //config_ = config;
  
  return OK;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request, Response* response) {

  std::string body = "<html><body><h1>404 Not Found</h1></body></html>"; 
  response->SetStatus(response->ResponseCode::NOT_FOUND);
  response->AddHeader("Content-Type", "text/plain");
  response->SetBody(body);


  return OK;
}