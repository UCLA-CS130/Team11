#define TEST_SERVER

#include <iostream>
#include <gmock/gmock.h>

#include "gtest/gtest.h"
#include "config_parser.h"
#include "server.h"
#include "server_config.h"

/* TODO: Redo test with common api
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

TEST_F(ParseRequestTest, InvalidRequest) {
  ParsedRequest req(request_buffer, request_buffer_size);
  Server s;
  const char* config_file = "demo_config";
  s.init(config_file); 
  ASSERT_FALSE(s.parse_request(invalid_request_line, &req));
}

TEST_F(ParseRequestTest, ValidRequest) {
  ParsedRequest req(request_buffer, request_buffer_size);
  Server s;
  const char* config_file = "demo_config";
  s.init(config_file); 
  ASSERT_TRUE(s.parse_request(valid_request_line, &req));
  EXPECT_EQ("GET", req.method);
  EXPECT_EQ("/echo", req.URI);
  EXPECT_EQ("/", req.path);
  EXPECT_EQ("echo", req.file);
  EXPECT_EQ("text/plain", req.mime_type);
}

TEST_F(ParseRequestTest, ValidFileRequest) {
  ParsedRequest req(request_buffer, request_buffer_size);
  Server s;
  const char* config_file = "demo_config";
  s.init(config_file); 
  ASSERT_TRUE(s.parse_request(file_request_line, &req));
  EXPECT_EQ("/static1", req.path);
  EXPECT_EQ("seal.jpg", req.file);
  EXPECT_EQ("image/jpeg", req.mime_type);
}
*/




