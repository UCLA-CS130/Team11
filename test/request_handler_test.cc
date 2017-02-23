#define TEST_REQUEST_HANDLER

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "gtest/gtest.h"
#include "server.h"
#include "request_handler.h"
#include "http_constants.h"
#include "config_parser.h"
#include "request.h"

using boost::asio::ip::tcp;

// TODO: use a mock for Request

TEST(RequestHandlerTest, EchoTest)
{
	const std::string echo_uri = "/echo";

	EchoHandler echo_handler;
	NginxConfig config;

	EXPECT_EQ(RequestHandler::Status::OK, echo_handler.Init(echo_uri,config));
}


