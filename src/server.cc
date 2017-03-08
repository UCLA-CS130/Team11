#include "server.h"
#include "http_constants.h"

// TODO: change initialization of threads
Server::Server() : acceptor_(io_service_), server_config_(nullptr), num_threads_(5) {}

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

  if (server_config_->get_num_threads() == -1) {
    BOOST_LOG_TRIVIAL(warning) << "Number of threads not found. Set to default of 5."; 
  }
  else {
    num_threads_ = server_config_->get_num_threads();
  }

  return true;
}

void processConnection(tcp::socket socket, ServerConfig *server_config_) {
  // Perform read
  char req_buffer[MAX_LENGTH];
  boost::system::error_code err;
  socket.read_some(boost::asio::buffer(req_buffer), err);

  if (err == boost::asio::error::eof) {
    BOOST_LOG_TRIVIAL(warning) << "Reached end of file";
  }
  else if (err) {
    throw boost::system::system_error(err);
  }

  std::string string_req(req_buffer);
  Request r(string_req);
  auto parsed_request = r.Parse(string_req);

  if (parsed_request == nullptr) {
    BOOST_LOG_TRIVIAL(warning) << "Malformed request. Ignoring request";
  }

  BOOST_LOG_TRIVIAL(info) << "Received the following request:";
  parsed_request->print_contents();

  // Route request: 
  // In the event of root prefixes, attempt to find matching handler by uri() first:
  std::shared_ptr<RequestHandler> handler = server_config_->get_handler(parsed_request->uri());

  // Attempt to find the longest matching prefix
  // We are calling by path() in anticipation for serving a file      
  if (handler == nullptr) {
    std::string longest_prefix = server_config_->find_longest_matching_prefix(parsed_request->path()); 
    BOOST_LOG_TRIVIAL(info) << "Longest matching prefix:" << longest_prefix;

    handler = server_config_->get_handler(longest_prefix); 
    if (handler == nullptr) {
      // Attempt to call by uri() instead (this is for requests one level deep - ie /status, /echo
      handler = server_config_->get_handler(parsed_request->uri()); 

      // If that still fails, handler does not exist
      if (handler == nullptr) {
        BOOST_LOG_TRIVIAL(warning) << "Handler not found. Calling NotFoundHandler";
        StatusCount::get_instance().statuses_map_[parsed_request->uri()][404]++;
        handler = server_config_->get_handler("404");
      }
    }
  }
  // Handle Request
  Response resp; 
  RequestHandler::Status request_status = handler->HandleRequest(*parsed_request, &resp); 

  StatusCount::get_instance().request_count_++;
  if (request_status == RequestHandler::Status::OK) {
    BOOST_LOG_TRIVIAL(info) << "Handle request OK";
    BOOST_LOG_TRIVIAL(info) << "Putting status in map";

    StatusCount::get_instance().statuses_map_[parsed_request->uri()][resp.GetStatus()]++;
    
    std::string req_to_write = resp.ToString();
    boost::asio::write(socket, boost::asio::buffer(req_to_write.c_str(), req_to_write.size()));
    StatusCount::get_instance().handlers_map_[parsed_request->uri()] = handler->GetName();
  }
  else {
    BOOST_LOG_TRIVIAL(warning) << "Error with handling request"; 

    // TODO: Change to output more than just 404 responses, get rid of hardcoding

    StatusCount::get_instance().statuses_map_[parsed_request->uri()][404]++;
    StatusCount::get_instance().handlers_map_[parsed_request->uri()] = "-> Failed, Sent to NotFoundHandler";

    // TODO: We should probably make this a series of if statements due to the specific nature of some
    // of the statuses 
    
    Response resp404; 
    handler = server_config_->get_handler("404");
    request_status = handler->HandleRequest(*parsed_request, &resp404); 
    std::string req_to_write = resp404.ToString();
    BOOST_LOG_TRIVIAL(info) << "The response being written";
    boost::asio::write(socket, boost::asio::buffer(req_to_write.c_str(), req_to_write.size()));
    
  }
}

void Server::listen() {
  // source for thread help: http://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp
  try {
    while(true) {
      // Accept connection 
      tcp::socket socket(io_service_);
      acceptor_.accept(socket);
      std::thread(processConnection, std::move(socket), server_config_).detach();
    }
  }
  catch (std::exception& e)
  {
    BOOST_LOG_TRIVIAL(fatal) << e.what(); 
  }
}
