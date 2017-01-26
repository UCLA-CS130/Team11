#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "serve_server.h"
#include "serve_response.h"


using boost::asio::ip::tcp;

const int MAX_LENGTH = 1024;

Header make_header(std::string name, std::string value) {
	Header h; 
	h.name = name; 
	h.value = value;
	return h;
}

Server::Server(unsigned short port) : acceptor_(io_service_)
{
	// Setup for accepting connection, taken from Boost sample documentation
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
  serve();
}

Server::~Server() {
	// TODO: Setup deconstructor
}

void Server::serve(){

	try {
		while(true) {
			// [1] Accept connection:
			tcp::socket socket(io_service_);
    	acceptor_.accept(socket);

			// [2] Perform read:
			char req_buffer[MAX_LENGTH];
			boost::system::error_code err;
			socket.read_some(boost::asio::buffer(req_buffer), err);
			if (err == boost::asio::error::eof)
        break; 
      else if (err)
        throw boost::system::system_error(err); 

      std::cout << "// REQUEST RECEIVED // " << std::endl;
     	std::cout << req_buffer << std:: endl; 

			// [3] Perform write: Creates a response object that builds the request response
      Response resp;
      resp.headers.push_back(make_header("Content-Type","text/plain")); 
      std::string body(req_buffer);
      std::string resp_headers = resp.response_builder("HTTP/1.1 200 OK", body);
      std::size_t resp_len = resp_headers.size();

      // Copy over response string to char array
      char data[resp_len];
      resp_headers.copy(data, resp_len);

      std::cout << "// RESPONSE SENT //" << std::endl;
      std::cout << data << std::endl;

			boost::asio::write(socket, boost::asio::buffer(data, resp_len));
		}
	}
	catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
	
}
