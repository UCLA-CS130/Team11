#define TEST_SERVER

#include <iostream>
#include <gmock/gmock.h>

#include "gtest/gtest.h"
#include "../src/config_parser.h"
#include "../src/server.h"
#include "../src/server_config.h"

class MockServerConfig : public ServerConfig {
public:
  MOCK_METHOD1(parse_config, bool(const char* arg));
};

class ParseRequestTest : public ::testing::Test {
protected:
  char* valid_request_line = "GET /echo HTTP/1.1\r\n";
  char* invalid_request_line = "GET HTTP/1.1\r\n";
  char* file_request_line = "GET /static1/seal.jpg HTTP/1.1\r\n";

  // Dummy values for ParsedRequest object 
  char* request_buffer = "";
  std::size_t request_buffer_size = 0;
};

TEST(ServerInitTest, NullServerConfig) {
  Server s;
  const char* config_file = "";
  ASSERT_FALSE(s.init(config_file));  
}


TEST(ServerInitTest, ValidServerConfig) {
  Server s;
  const char* config_file = "new_config";
  ASSERT_TRUE(s.init(config_file));
}


TEST_F(ParseRequestTest, InvalidRequest) {
  Request req(request_buffer);
  Server s;
  const char* config_file = "new_config";
  s.init(config_file); 
  ASSERT_FALSE(req.Parse(invalid_request_line));
}

