#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server
{
public:

  Server(boost::asio::io_service& io_service, int port);
  ~Server();

private:
  tcp::acceptor acceptor_;
  tcp::endpoint endpoint_;
  tcp::socket socket_;

  void handle_accept();
    
};