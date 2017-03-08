#ifndef TEST_STATUS
#define TEST_STATUS
#endif

#include "gtest/gtest.h"
#include "../src/status_count.h"

TEST(StatusCountTest, HandlersMapTest)
{
	std::string static_uri = "/static1";
	std::string echo_uri = "/echo";
	StatusCount::get_instance().handlers_map_[static_uri] = "StaticHandler";
	StatusCount::get_instance().handlers_map_[echo_uri] = "EchoHandler";

	std::string result;

	for(auto const &i : StatusCount::get_instance().handlers_map_) {
    	result += i.first + " " + i.second + "\n ";
  	}

  	EXPECT_EQ(result, "/echo EchoHandler\n /static1 StaticHandler\n ");
}
