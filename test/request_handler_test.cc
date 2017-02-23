#include "gtest/gtest.h"
#include "request_handler.h"
#include "request.h"
#include "response.h"
#include "http_constants.h"

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

