#include "server.h"
#include "http_constants.h"

Server::Server() : acceptor_(io_service_), server_config(nullptr) {}

Server::~Server() {
  if (server_config) {
    delete server_config;
  }
  std::cout << "Destructing server" << std::endl;
}

void Server::init_acceptor() {
  // Setup for accepting connection, taken from Boost sample documentation
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), server_config->get_port());
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
}

bool Server::init(const char* config_file) {

  // Initialize the server with the port it will be listening from
  server_config = new ServerConfig(); 

  if (!server_config->parse_config(config_file)) {
    BOOST_LOG_TRIVIAL(fatal) << "Malformed config file";
    return false; 
  }
  else if (server_config->get_port() == -1 || server_config->get_port() == MAX_PORT_NUM) {
    BOOST_LOG_TRIVIAL(fatal) << "Port number not found or port number exceeds max value of " << MAX_PORT_NUM; 
    return false; 
  }

  init_acceptor(); 

  // Construct the request handlers
  if(!server_config->build_handlers()) {
    BOOST_LOG_TRIVIAL(fatal) << "Zero handlers were specified or successfully built";
    return false; 
  }
  return true;
}
