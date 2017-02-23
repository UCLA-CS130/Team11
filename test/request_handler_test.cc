#define TEST_REQUEST_HANDLER

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include "server.h"
#include "request_handler.h"
#include "http_constants.h"
#include "config_parser.h"
#include "request.h"

using boost::asio::ip::tcp;

// TODO: use a mock for Request

// class MockRequest : public Request {
//   public:
// 	MOCK_CONST_METHOD0(raw_request, std::string());
// };

TEST(RequestHandlerTest, EchoTest)
{
	const std::string echo_uri = "/echo";

	EchoHandler echo_handler;
	NginxConfig config;

	EXPECT_EQ(RequestHandler::Status::OK, echo_handler.Init(echo_uri,config));
}

// TEST(RequestHandlerTest, StaticTest)
// {

// }

// Test Fixture
/* TODO: Redo tests with common API
class RequestHandlerTest : public ::testing::Test {

    protected:
        void BuildParsedRequest() {
            request_buffer = "";
			request_buffer_size = 0;
			parsed_request = new ParsedRequest(request_buffer, request_buffer_size);
        }

		char* request_buffer;
		std::size_t request_buffer_size;
		ParsedRequest *parsed_request;
}; 

// Empty Request should return false 
TEST_F(RequestHandlerTest, HandleEmptyEchoRequest)
{
	BuildParsedRequest();
	std::map<std::string, std::string> uri_map;
	Response server_response;
	EchoRequestHandler echo_request_handler(parsed_request, uri_map, &server_response);
	ASSERT_FALSE(echo_request_handler.handle_request());
}

// Echo Request with URI set to ECHO_REQUEST and empty input
TEST_F(RequestHandlerTest, HandleSimpleEchoRequest)
{
	BuildParsedRequest();
	std::map<std::string, std::string> uri_map;
	Response server_response;
	parsed_request->URI = ECHO_REQUEST;
	EchoRequestHandler echo_request_handler(parsed_request, uri_map, &server_response);
	ASSERT_TRUE(echo_request_handler.handle_request())
	<< "Echo request handled with no issues";
}

// Echo Request with URI set to DEFAULT_REQUEST and empty input
TEST_F(RequestHandlerTest, HandleDefaultRequest)
{
	BuildParsedRequest();
	std::map<std::string, std::string> uri_map;
	Response server_response;
	parsed_request->URI = DEFAULT_REQUEST;
	// header value should be HTML
	EchoRequestHandler echo_request_handler(parsed_request, uri_map, &server_response);
	ASSERT_TRUE(echo_request_handler.handle_request())
	<< "Default request handled with no issues";
	EXPECT_EQ(echo_request_handler.headers.back().value_, HTML)
	<< "Content type set to HTML";
}

// Static Request with Bad Path should return false
TEST_F(RequestHandlerTest, StaticHandleBadPath1)
{
	BuildParsedRequest();
	std::map<std::string, std::string> uri_map;
	Response server_response;
	server_response.server = "my_server";
	StaticRequestHandler static_request_handler(parsed_request, uri_map, &server_response);
	ASSERT_FALSE(static_request_handler.handle_request())
	<< "File does not exist";
}

// Static Request with Bad Path should return false
TEST_F(RequestHandlerTest, StaticHandleBadPath2)
{
	BuildParsedRequest();
	std::map<std::string, std::string> uri_map;
	Response server_response;
	server_response.server = "my_server";
	StaticRequestHandler static_request_handler(parsed_request, uri_map, &server_response);
	static_request_handler.handle_request();
	ASSERT_EQ(server_response.status, NOT_FOUND)
	<< "Status set to not found";
}

// Build a header using write headers and compare to expected string
TEST_F(RequestHandlerTest, BuildHeader)
{
	BuildParsedRequest();
	char* expected_response_buffer = "HTTP/1.1 200 OK\r\nServer: Serve 2.0\r\nContent-Type: text/plain\r\n\r\n";
	parsed_request->URI = ECHO_REQUEST;
	parsed_request->HTTP = "HTTP/1.1";
	parsed_request->mime_type = "text/plain";
	parsed_request->print_contents();
	
	std::map<std::string, std::string> uri_map;
	Response server_response;
	EchoRequestHandler echo_request_handler(parsed_request, uri_map, &server_response);

	echo_request_handler.handle_request();
	std::string response_buffer = echo_request_handler.build_headers();
	ASSERT_EQ(response_buffer, expected_response_buffer)
	<< "Header built correctly";
}
*/

