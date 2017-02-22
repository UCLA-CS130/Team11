#include "gtest/gtest.h"
#include "response.h"

TEST(FirstTest, ToString) {
 // EXPECT_TRUE(success) << "Valid config files should be parsed successfully";
  //EXPECT_EQ(2, out_config.statements_.size());
  //EXPECT_EQ(3, out_config.statements_[1]->child_block_->statements_.size());
	Response r;
	r.SetStatus(Response::ResponseCode::OK);
	r.AddHeader("dog", "is cool");
	r.SetBody("blah");
	EXPECT_FALSE(r.ToString() == "dog is cool");
}

TEST(SecondTest, AnotherToString) {

  Response r1;
  Response r2;

  std::string format = "Content-Type: text/plain\r\nContent-Length: 5\r\n\r\ni love lamp";

  std::string response_from_200 = "HTTP/1.0 200 OK\r\n" + format;

  r1.SetStatus(Response::ResponseCode::OK);
  r1.AddHeader("Content-Type", "text/plain");
  r1.AddHeader("Content-Length", "5");
  r1.SetBody("i love lamp");
	
  std::string format2 = "Content-Type: text/plain\r\nContent-Length: 5\r\n\r\ni hope this works";


  std::string response_from_400 = "HTTP/1.0 403 FORBIDDEN\r\n" + format2;

  r2.SetStatus(Response::ResponseCode::FORBIDDEN);
  r2.AddHeader("Content-Type", "text/plain");
  r2.AddHeader("Content-Length", "5");
  r2.SetBody("i hope this works");
	
  EXPECT_EQ(response_from_400, r2.ToString());

   //EXPECT_EQ(response_from_400, r2.ToString());



}
