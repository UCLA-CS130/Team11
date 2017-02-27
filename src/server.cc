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

  spawn_threads();

  return true;
}

void Server::spawn_threads() {
  // sources: http://stackoverflow.com/questions/19500404/how-to-create-a-thread-pool-using-boost-in-c
  // http://stackoverflow.com/questions/8127080/boost-asio-multithreaded-tcp-synchronous-server
  BOOST_LOG_TRIVIAL(info) << "Creating threads";
  for(int i = 0; i < num_threads_; i++){
    threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
  }
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
          BOOST_LOG_TRIVIAL(warning) << "Handler not found. Calling NotFoundHandler";
          StatusCount::get_instance().statuses_map_[parsed_request->uri()][404]++;
          handler = server_config_->get_handler("404");
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

        // We should probably make this a series of if statements due to the specific nature of some
        // of the statuses 
        
        Response resp404; 
        handler = server_config_->get_handler("404");
        request_status = handler->HandleRequest(*parsed_request, &resp404); 
        std::string req_to_write = resp404.ToString();
        BOOST_LOG_TRIVIAL(info) << "The response being written";
        boost::asio::write(socket, boost::asio::buffer(req_to_write.c_str(), req_to_write.size()));
        
      }
    }
     /*
   * This will stop the io_service processing loop. Any tasks
   * you add behind this point will not execute.
    */
   BOOST_LOG_TRIVIAL(info) << "Stopping io service";
    io_service_.stop();
    /*
   * Will wait till all the threads in the thread pool are finished with 
   * their assigned tasks and 'join' them. Just assume the threads inside
   * the threadpool will be destroyed by this method.
   */
    threadpool.join_all();
  }
  catch (std::exception& e)
  {
    BOOST_LOG_TRIVIAL(fatal) << e.what(); 
  }
}
