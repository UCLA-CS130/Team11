#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <map>
#include <iostream>
#include "config_parser.h"

struct ServerConfig 
{
	ServerConfig();
	virtual ~ServerConfig();
	std::map<std::string, std::string> uri_map;
	short port_num;
	virtual bool parse_config(const char* arg);
};


#endif