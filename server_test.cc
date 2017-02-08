#define TEST_SERVER

#include <iostream>
#include <gmock/gmock.h>

#include "gtest/gtest.h"
#include "config_parser.h"
#include "server.h"
#include "server_config.h"
#include "server_containers.h"

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

TEST(ServerExtensionToTypeTest, ReturnValidTypeTest) {
  Server s; 
  const char* config_file = "demo_config";
  s.init(config_file);
  EXPECT_EQ("text/html", s.extension_to_type(".html"));
  EXPECT_EQ("text/html", s.extension_to_type(".htm"));
  EXPECT_EQ("image/png", s.extension_to_type(".png"));
  EXPECT_EQ("image/jpeg", s.extension_to_type(".jpg"));
  EXPECT_EQ("image/jpeg", s.extension_to_type(".jpeg"));
  EXPECT_EQ("image/gif", s.extension_to_type(".gif"));   
}

TEST(ServerExtensionToTypeTest, ReturnDefaultTypeTest) {
  Server s; 
  const char* config_file = "demo_config";
  s.init(config_file);
  EXPECT_EQ("text/plain", s.extension_to_type(".txt"));
  EXPECT_EQ("text/plain", s.extension_to_type(".zip"));
  EXPECT_EQ("text/plain", s.extension_to_type(""));
  EXPECT_EQ("text/plain", s.extension_to_type("-1"));
}





