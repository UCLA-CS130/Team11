#include "gtest/gtest.h"
#include "server_config.h"

TEST(ServerConfigTest, IsValidFile) {

  ServerConfig s;
  s.parse_config("demo_config");
  EXPECT_TRUE(s.port_num == 9999);
  EXPECT_TRUE(s.uri_map["/static1"] == "example_dir");
  EXPECT_TRUE(s.uri_map["/static2"] == "example_dir_2");
 
}


TEST(ServerConfigTest2, IsInvalidPortNumber) {
  //When there is no valid port number specified 
  ServerConfig s;
  EXPECT_FALSE(s.parse_config("invalid_configs/invalid_config"));
  
}

TEST(ServerConfigTest3, IsInvalidPathName) {
  //When there is an invalid path name 
  ServerConfig s;
  EXPECT_TRUE(s.parse_config("invalid_configs/bad_path_config"));
  
}

TEST(ServerConfigTest4, IsInvalidLocation) {
  //When there is an invalid location
  ServerConfig s;
  EXPECT_TRUE(s.parse_config("invalid_configs/bad_location_config"));
  
}