#define TEST_SERVER_CONFIG

#include <iostream>
#include <gmock/gmock.h>

#include "gtest/gtest.h"
#include "config_parser.h"
#include "server.h"
#include "server_config.h"

class MockServerConfig : public ServerConfig {
public:
  MOCK_METHOD1(parse_config, bool(const char* arg));
};


TEST(ServerInitTest, NullServerConfig) {
  Server s;
  const char* config_file = "";
  s.server_config = nullptr;
  ASSERT_FALSE(s.init(config_file));  
}

TEST(ServerInitTest, ServerConfigError) { 
  MockServerConfig m;
  Server s;
  s.server_config = &m; 
  const char* config_file = "";
  ASSERT_FALSE(s.init(config_file));
}

TEST(ServerInitTest, ValidServerConfig) {
  MockServerConfig m;
  Server s;
  s.server_config = &m;
  const char* config_file = "demo_config";
  ASSERT_TRUE(s.init(config_file));
}







