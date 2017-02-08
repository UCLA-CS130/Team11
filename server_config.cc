#include "server_config.h"

ServerConfig::ServerConfig() : port_num(-1) {}

/**
 * Parse config file for port number and static file paths
 * @param  arg Config file
 * @return     Returns true if config file is valid and parse is successful
 *             Returns false if invalid config file
 */
bool ServerConfig::parse_config(const char* arg)
{
  NginxConfig config;
  NginxConfigParser parser;
  std::string  port, listen_token, location_token, uri, path;

  if (!parser.Parse(arg, &config)) {
    std::cerr << "Unable to parse config file. Check formatting.\n"; 
    return false; 
  }

  // Loop through parsed tokens of config file
  for(int i = 0; i < config.statements_.size(); i++)
  {
    // Check for port number
    port = config.statements_[i]->tokens_[1];
   
    if(port != "")
    {
        if(isdigit(port[0]))
        {

          listen_token = config.statements_[i]->tokens_[0];

          if(listen_token == "listen")
          {
            port_num = (short) stoi(config.statements_[i]->tokens_[1]);
          }

          else 
          {
            std::cerr << "Incorrect configuration for port: " << listen_token << std::endl;
            std::cerr << "Specify port number as: listen [port];" << std::endl;
            //testing::internal::CaptureStdout();
            //std::cout << "port number not specified" std::endl;
            //std::string output = testing::internal::GetCapturedStdout();
          }
        }
        // Check for location 
        else 
        {
          uri = config.statements_[i]->tokens_[1];
          path = config.statements_[i]->child_block_->statements_[0]->tokens_[1];

          location_token = config.statements_[i]->tokens_[0];

          if(location_token == "location")
          {
            if (uri[0] == '/')
            {
              uri_map[uri] = path;
            }

            else
            {
              std::cerr << "Incorrect format for location block " << uri << " " << path << std::endl;
            }
          } 

          else
          {
            std::cerr << "Configuration not supported: " << location_token << std::endl;

          }
        }
      }
    }
  // Verify port number is set
  if (port_num == -1) 
  {
    std::cerr << "Error: Port number not specified." << std::endl;
    return false;
  }

  return true;
}