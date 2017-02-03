#include <cstdio>
#include <fstream>
#include "gtest/gtest.h"
#include "config_parser.h"
#include "serve_server.h"
#include "serve_response.h"


TEST(HeaderTest, ValidHeader)
{ 
  Header h = make_header("stephen", "0");
  ASSERT_TRUE(h.name == "stephen");
  ASSERT_TRUE(h.value == "0");		   
}


