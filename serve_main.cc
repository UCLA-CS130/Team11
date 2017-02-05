#include <cstdlib>
#include <iostream>
#include "server.h"
#include "config_parser.h"
int main(int argc, char* argv[]) {

  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    NginxConfig config;
    NginxConfigParser parser;
    parser.Parse(argv[1], &config);

    std::string p = config.statements_[0]->tokens_[1];
    Server s((short)std::stoi(p));
    s.listen();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}


