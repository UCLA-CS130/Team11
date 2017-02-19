#include "server.h"

Server::~Server() {
	std::cout << "Destructing server" << std::endl;
}

bool Server::init(const char* config_file) {
	return true;
}

RequestHandler* Server::get_handler(std::string) {
	return nullptr; 
}