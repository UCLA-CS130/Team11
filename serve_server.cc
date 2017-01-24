#include <boost/asio.hpp>

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_service& io_service, int port) {
  endpoint_ = tcp::endpoint(tcp::v4(), port); 
  acceptor_ = tcp::acceptor(io_service, endpoint_);
  handle_accept();
}

void Server::handle_accept(){

}
