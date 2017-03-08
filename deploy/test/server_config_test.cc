#ifndef TEST_SERVER
#define TEST_SERVER
#endif

#include "gtest/gtest.h"
#include "../src/server_config.h"

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

// TEST(WellFormedURITest, ValidURI) {
//   ServerConfig s; 
//   EXPECT_TRUE(s.well_formed_uri("/static1/foo")); 
// }

// TEST(WellFormedURITest, InvalidURI) {
//   ServerConfig s; 
//   EXPECT_FALSE(s.well_formed_uri("static1"));
//   EXPECT_FALSE(s.well_formed_uri("static1/"));
//   EXPECT_FALSE(s.well_formed_uri("/static1//foo"));
// }
