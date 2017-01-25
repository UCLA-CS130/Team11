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
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
  serve();
}

Server::~Server() {

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
			size_t length = socket.read_some(boost::asio::buffer(req_buffer), err);
			if (err == boost::asio::error::eof)
        break; 
      else if (err)
        throw boost::system::system_error(err); 

			// [3] Perform write:
      Response resp;
      resp.headers.push_back(make_header("HTTP/1.1 200 OK\r\n","")); 
      resp.headers.push_back(make_header("Content-Type","text/plain")); 
      std::string resp_headers = resp.header_builder();
            
      std::size_t resp_len = length + resp_headers.size();
      char data[resp_len];

      resp_headers.copy(data, resp_headers.size());
      std::memcpy(&data[resp_headers.size()], req_buffer, length);

      data[resp_len] = '\0';

      std::cout << "this is the buffer: " << data << std::endl;

			boost::asio::write(socket, boost::asio::buffer(data, resp_len));
		}
	}
	catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
	
}
