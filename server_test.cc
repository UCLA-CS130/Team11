#include "gtest/gtest.h"
#include "config_parser.h"
#include "server.h"
#include "server_config.h"


TEST(ServerInitTest, NullServerConfig) {
	Server s;
	const char* config_file = "";
	s.server_config = nullptr;
	ASSERT_FALSE(s.init(config_file));  
}



