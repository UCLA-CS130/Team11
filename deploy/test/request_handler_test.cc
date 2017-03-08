#define TEST_REQUEST_HANDLER

#include "gtest/gtest.h"
#include "../src/request_handler.h"
#include "../src/request.h"
#include "../src/response.h"
#include "../src/http_constants.h"
#include "../src/config_parser.h"

TEST(RequestHandlerTest, EchoTest)
{
	const std::string echo_uri = "/echo";

	EchoHandler echo_handler;
	NginxConfig config;

	EXPECT_EQ(RequestHandler::Status::OK, echo_handler.Init(echo_uri,config));
}

TEST(EchoHandleRequest, NullResponse) {
	EchoHandler echo; 
	Request req("dummy request"); 
	RequestHandler::Status status = echo.HandleRequest(req, nullptr);
	EXPECT_EQ(status, RequestHandler::Status::INVALID_RESPONSE); 
}

TEST(EchoHandleRequest, ValidHandleRequest) {
	EchoHandler echo; 
	Request req("dummy request");
	Response resp; 
	RequestHandler::Status status = echo.HandleRequest(req, &resp);
	EXPECT_EQ(status, RequestHandler::Status::OK); 
}

TEST(NotFoundRequest, NotFound)
{
	NotFoundHandler nf;
	Request req("dummy request");
	Response resp;
	RequestHandler::Status status = nf.HandleRequest(req, &resp);

	std::string format = "Content-Type: text/html\r\n\r\n";
	std::string response = "HTTP/1.1 404 Not Found\r\n";
	std::string body = "<html><body><h1>404 Not Found</h1></body></html>";

	std::string result = response + format + body;

	EXPECT_EQ(status, RequestHandler::Status::OK);
	EXPECT_EQ(result, resp.ToString());
}
