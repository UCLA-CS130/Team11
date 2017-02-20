#include "gtest/gtest.h"
#include "server_config.h"

TEST(ServerConfigTest, GetPortNumber) {
  ServerConfig s;
  EXPECT_TRUE(s.parse_config("new_config"));
  EXPECT_TRUE(s.get_port() == 9999);
}

TEST(ServerConfigTest, MissingPortNumber) {
  ServerConfig s;
  EXPECT_TRUE(s.parse_config("invalid_configs/missing_port"));
  EXPECT_TRUE(s.get_port() == -1);
}
