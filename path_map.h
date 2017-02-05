#ifndef PATH_MAP_H
#define PATH_MAP_H
#include <map>
#include <iostream>
#include "config_parser.h"

struct PathMap 
{
	std::map<std::string, std::string> request;
	std::string portNum;
	void URIPathMap(const char* arg);
};


#endif