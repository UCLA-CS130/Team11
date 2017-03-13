#include "gtest/gtest.h"
#include "../src/request.h"

TEST(RequestTest, ParseRequestTest)
{
	const std::string r_request = "GET /echo HTTP/1.1\r\nHost: localhost:8011\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:31.0) Gecko/20100101 Firefox/31.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-gb,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive";

	Request req(r_request);

	std::unique_ptr<Request> parsed_request;

	parsed_request = req.Parse(r_request);

	EXPECT_EQ("GET", parsed_request->method());
	EXPECT_EQ("/echo", parsed_request->uri());
	EXPECT_EQ("HTTP/1.1", parsed_request->version());
	EXPECT_EQ(parsed_request->headers().size(), 6);

}
