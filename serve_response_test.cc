#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <map>
#include "serve_server.h"
#include "serve_response.h"
#include <fstream>
#include <iostream>
#include "gtest/gtest.h"
#include "config_parser.h"

TEST(ResponseTest, FirstResponse)
{
  Response r;
  std::string s = r.response_builder("Not Valid", "FirstResponse");
  ASSERT_FALSE(s == "http");
}

TEST(ResponserTest, SecondResponse)
{
  Response r;
  std::string s = r.response_builder("Not Valid", "FirstResponse");
  ASSERT_TRUE(s == "Not Valid\r\n\r\nFirstResponse");
}
