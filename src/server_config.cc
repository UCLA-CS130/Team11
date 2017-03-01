#include "server_config.h"
#include "http_constants.h"

ServerConfig::~ServerConfig() {
  if (config_) {
    delete config_; 
  }
}

bool ServerConfig::parse_config(const char* arg)
{
  config_ = new NginxConfig; 
  NginxConfigParser parser;

  if (!parser.Parse(arg, config_)) {
    std::cerr << "Unable to parse config file. Check formatting.\n"; 
    return false; 
  }
  return true;
}

int ServerConfig::get_port() {
  std::string token;
  std::string value; 
  for (unsigned int i = 0; i < config_->statements_.size(); i++) {
    std::vector<std::string> token_list = config_->statements_[i]->tokens_; 
    if (token_list.size() < 2) {
      continue; 
    }
    token = token_list[0]; 
    value = token_list[1];
    if (token == PORT) {
      if (isdigit(value[0])) {
        port_ = std::stoi(value); 
      }
    }
  }
  return port_; 
}

int ServerConfig::get_num_threads() {
  std::string token;
  std::string value;
  for (unsigned int i = 0; i < config_->statements_.size(); i++) {
    std::vector<std::string> token_list = config_->statements_[i]->tokens_; 
    if (token_list.size() < 2) {
      continue; 
    }
    token = token_list[0]; 
    value = token_list[1];
    if (token == NUM_THREADS) {
      if (isdigit(value[0])) {
        num_threads_ = std::stoi(value); 
      }
    }
  }
  return num_threads_; 
}

bool ServerConfig::well_formed_uri(std::string uri) {
  int size = uri.size();
  if (uri[0] != '/' || uri[size-1] == '/') {
    return false; 
  }
  // Check for repeated '/' in a row
  for (int i = 0; i < size; i++) {
    if (uri[i] == '/' && i != (size-1)) {
      if (uri[i+1] == '/') {
        return false; 
      }
    }
  }
  return true;
}

bool ServerConfig::get_handler_config(std::shared_ptr<NginxConfigStatement>& statement, std::unique_ptr<NginxConfig>& handler_config) {
  if (statement->child_block_ == nullptr) {
    return false; 
  }
  handler_config = std::move(statement->child_block_); 
  return true;
}

std::string ServerConfig::handler_map_content() {
  std::string content = "";
  for (const auto &p : handler_map_) {
   content += "handler_map_[" + p.first + "] = " + p.second->uri() + "\n";
  } 
  return content;
}

bool ServerConfig::build_handlers() {
  for (unsigned int i = 0; i < config_->statements_.size(); i++) {
    std::vector<std::string> token_list = config_->statements_[i]->tokens_;

    // Handle default handler: 
    if (token_list.size() == 2) {

      std::string t = token_list[0];
      std::string v = token_list[1];

      if(t == DEFAULT  && v == NOT_FOUND_HANDLER) {

        std::unique_ptr<NginxConfig> tmp_config; 
        auto req_handler = RequestHandler::CreateByName(v.c_str()); 

        if(req_handler != nullptr)
        {
          req_handler->Init("Default 404 Handler", *tmp_config);
          std::shared_ptr<RequestHandler> tmp(req_handler);
          handler_map_["404"] = tmp;
          BOOST_LOG_TRIVIAL(info) << "default handler successfully called"; 
        }
      }
    }
    else if (token_list.size() == 3) {
      std::string token = token_list[0];
      std::string uri = token_list[1]; 
      std::string handler = token_list[2];

      if(token == PATH)
      {

        // Manage URI 
        if(!well_formed_uri(uri)) {
          BOOST_LOG_TRIVIAL(warning) << uri << " is not well formed. Ignoring path block.";
          continue;  
        }

        auto it = handler_map_.find(uri); 
        // Check if same exact uri exists
        if (it != handler_map_.end()) {
          BOOST_LOG_TRIVIAL(warning) << uri << " exists already. Ignoring path block";
          continue;
        }
        else {

        }

        // Pass in config block to handler
        std::unique_ptr<NginxConfig> handler_config; 
        if (!get_handler_config(config_->statements_[i], handler_config)) {
          BOOST_LOG_TRIVIAL(warning) << handler << " is missing child config block {...}. Ignoring path block.";
          continue;
        }

        auto req_handler = RequestHandler::CreateByName(handler.c_str()); 

        // TODO: Incorporate the 'default NotFoundHandler{}'
        if (req_handler == nullptr) {
          BOOST_LOG_TRIVIAL(warning) << handler << " is not implemented. Ignoring path block.";
          //NotFoundHandler
          //RequestHandler::Status init = req_handler->Init(uri, *handler_config);

          continue; 
        }

        RequestHandler::Status init = req_handler->Init(uri, *handler_config);
        
        if (init == RequestHandler::OK) {
          BOOST_LOG_TRIVIAL(info) << "Init function successfully called"; 
        }
        else if (init == RequestHandler::MISSING_ROOT) {
          BOOST_LOG_TRIVIAL(warning) << "StaticHandler missing root path. Ignoring path block";
          continue; 
        }
        else if (init == RequestHandler::INVALID_PATH) {
          BOOST_LOG_TRIVIAL(warning) << "StaticHandler path is invalid. Ignoring path block";
          continue;
        }

        std::shared_ptr<RequestHandler> tmp(req_handler);
        handler_map_[uri] = tmp;
      }
    }
  }

  if (handler_map_.size() == 0) {
    return false; 
  }

  auto it = handler_map_.find("404"); 
  if (it == handler_map_.end()) {
    BOOST_LOG_TRIVIAL(fatal) << "NotFoundHandler not defined, 404 requests will not be handled!";
  }
  
  BOOST_LOG_TRIVIAL(info) << "Handler map content: \n" << handler_map_content();
  return true;
}

std::string ServerConfig::find_longest_matching_prefix(std::string uri) {
  std::string prefix = uri;  
  while (prefix != "") {
    auto it = handler_map_.find(prefix); 
    // We found an exact match!
    if (it != handler_map_.end()) {
      return prefix; 
    }
    // Find next path
    boost::filesystem::path p(prefix);
    prefix = p.parent_path().string(); 
  }

  return "";
}

// Return exact match 
std::shared_ptr<RequestHandler> ServerConfig::get_handler(std::string uri) {
  auto it = handler_map_.find(uri);
  if (it != handler_map_.end()) {
     return it->second;
  }
  return nullptr;
}

