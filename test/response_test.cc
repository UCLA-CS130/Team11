#include "gtest/gtest.h"
#include "response.h"


TEST(ToStringTest, ToStringResponse) {
  Response r1;
  Response r2;

  std::string format = "Content-Type: text/plain\r\nContent-Length: 5\r\n\r\nbody";
  std::string response_from_200 = "HTTP/1.1 200 OK\r\n" + format;

  r1.SetStatus(Response::ResponseCode::OK);
  r1.AddHeader("Content-Type", "text/plain");
  r1.AddHeader("Content-Length", "5");
  r1.SetBody("body");

  std::string format2 = "Content-Type: text/plain\r\nContent-Length: 5\r\n\r\nbody";
  std::string response_from_400 = "HTTP/1.1 403 Forbidden\r\n" + format2;

  r2.SetStatus(Response::ResponseCode::FORBIDDEN);
  r2.AddHeader("Content-Type", "text/plain");
  r2.AddHeader("Content-Length", "5");
  r2.SetBody("body");
  
  EXPECT_EQ(response_from_200, r1.ToString());
  EXPECT_EQ(response_from_400, r2.ToString());
}