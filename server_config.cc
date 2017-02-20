#include "server_config.h"
#include "http_constants.h"

ServerConfig::~ServerConfig() {
  if (config_) {
    delete config_; 
  }
}
int ServerConfig::get_port() {
  std::string token;
  std::string value; 
  for (int i = 0; i < config_->statements_.size(); i++) {
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