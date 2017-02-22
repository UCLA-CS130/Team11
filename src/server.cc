#include "request_handler.h"
#include "status_count.h"
#include "server.h"
#include "http_constants.h"
#include <vector>

Server::Server() : acceptor_(io_service_), server_config_(nullptr) {}

Server::~Server() {
  if (server_config_) {
    delete server_config_;
  }
  std::cout << "Destructing server" << std::endl;
}

void Server::init_acceptor() {
  // Setup for accepting connection, taken from Boost sample documentation
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), server_config_->get_port());
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
}

bool Server::init(const char* config_file) {

  // Initialize the server with the port it will be listening from
  server_config_ = new ServerConfig(); 

  if (!server_config_->parse_config(config_file)) {
    BOOST_LOG_TRIVIAL(fatal) << "Malformed config file";
    return false; 
  }
  else if (server_config_->get_port() == -1 || server_config_->get_port() == MAX_PORT_NUM) {
    BOOST_LOG_TRIVIAL(fatal) << "Port number not found or port number exceeds max value of " << MAX_PORT_NUM; 
    return false; 
  }

  init_acceptor(); 

  // Construct the request handlers
  if(!server_config_->build_handlers()) {
    BOOST_LOG_TRIVIAL(fatal) << "Zero handlers were specified or successfully built";
    return false; 
  }
  return true;
}

void Server::listen() {
  try {
    while(true) {

      // Accept connection 
      tcp::socket socket(io_service_);
      acceptor_.accept(socket);

      // Perform read
      char req_buffer[MAX_LENGTH];
      boost::system::error_code err;
      socket.read_some(boost::asio::buffer(req_buffer), err);
      if (err == boost::asio::error::eof) {
        break; 
      }
      else if (err) {
        throw boost::system::system_error(err);
      }

      std::string string_req(req_buffer);
      Request r(string_req);
      auto parsed_request = r.Parse(string_req);

      if (parsed_request == nullptr) {
        BOOST_LOG_TRIVIAL(warning) << "Malformed request. Ignoring request";
        continue;
      }

      BOOST_LOG_TRIVIAL(info) << "Received the following request:";
      parsed_request->print_contents();

      // Route request:
      // Attempt to find the longest matching prefix
      // We are calling by path() in anticipation for serving a file      
      std::string longest_prefix = server_config_->find_longest_matching_prefix(parsed_request->path()); 
      BOOST_LOG_TRIVIAL(info) << "Longest matching prefix:" << longest_prefix;

      std::shared_ptr<RequestHandler> handler = server_config_->get_handler(longest_prefix); 
      if (handler == nullptr) {
        // Attempt to call by uri() instead (this is for requests one level deep - ie /status, /echo
        handler = server_config_->get_handler(parsed_request->uri()); 

        // If that still fails, handler does not exist
        if (handler == nullptr) {
          BOOST_LOG_TRIVIAL(warning) << "Handler not found. Ignoring request";
          continue;
        }
      }

      // Handle Request
      Response resp; 
      RequestHandler::Status request_status = handler->HandleRequest(*parsed_request, &resp); 

      // TODO: finish implementing
      // StatusCount::get_instance().handlers_map_[];

      StatusCount::get_instance().request_count_++;
      if (request_status == RequestHandler::Status::OK) {
        std::cout << "Handle request OK" << std::endl;

        std::cout << "Putting status in map" << std::endl;
        StatusCount::get_instance().statuses_map_[parsed_request->uri()][resp.GetStatus()]++;
        
        std::string req_to_write = resp.ToString();
        boost::asio::write(socket, boost::asio::buffer(req_to_write.c_str(), req_to_write.size()));
      }
      else {
        BOOST_LOG_TRIVIAL(warning) << "Error with handling request"; 
        // TODO: If request_status is not OK, evoke a different handler ie 404 
        // We should probably make this a series of if statements due to the specific nature of some
        // of the statuses 
        
        // By default, resp is set with a response code of OK, we should update to the correct response
        // code here 
      }
      
 
    }
  }
  catch (std::exception& e)
  {
    BOOST_LOG_TRIVIAL(fatal) << e.what(); 
  }
}
