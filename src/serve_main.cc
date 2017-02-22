#include <cstdlib>
#include <iostream>
#include "server.h"
int main(int argc, char* argv[]) {

  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: ./serve <config_file>\n";
      return 1;
    }

    Server s;
    
    if (s.init(argv[1]) == false) {
      std::cerr << "Failed to initialize server." << std::endl;
      exit(EXIT_FAILURE);
    }

    s.listen();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}


