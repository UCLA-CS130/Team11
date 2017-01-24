#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Connection
{
public:
  Connection(tcp::socket);
  ~Connection();

private:
  tcp::socket socket_; 
  int MAX_LENGTH = 1024; 
  
};