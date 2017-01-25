#include <cstdlib>
#include <iostream>
#include "serve_server.h"
int main(int argc, char* argv[]) {

  // TODO: Instead of manually entering port, utilize nginx config parser
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    Server s(std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
