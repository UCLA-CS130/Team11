#define TEST_REQUEST_HANDLER

#include "gtest/gtest.h"
#include "request_handler.h"
#include "request.h"
#include "response.h"
#include "http_constants.h"
#include "config_parser.h"
#include "request.h"

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

