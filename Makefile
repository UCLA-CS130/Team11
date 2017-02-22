# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++11 -g -Wall -DBOOST_LOG_DYN_LINK
TCFLAGS = --coverage
LIBFLAGS =  -lboost_system -lboost_filesystem -lboost_log -lpthread
SRC = config_parser.cc serve_main.cc server.cc request_handler.cc server_config.cc request.cc response.cc
GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock
TARGET= serve


$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBFLAGS) -o $(TARGET) 

config_parser_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	$(CC) -std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -I${GTEST_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc
	ar -rv libgtest.a gtest-all.o
	ar -rv libgmock.a gmock-all.o
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o config_parser_test

# TODO:
# server_test:
# $(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include server_test.cc server.cc server_config.cc config_parser.cc request_handler.cc response.cc request.cc${GMOCK_DIR}/src/gmock_main.cc libgtest.a libgmock.a $(LIBFLAGS) -o server_test

# request_handler_test:
# $(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include request_handler_test.cc server_config.cc config_parser.cc request_handler.cc response.cc request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o request_handler_test

# request_test
# Add testing coverage

server_config_test:
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include server_config_test.cc server.cc config_parser.cc request_handler.cc server_config.cc response.cc request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o server_config_test

response_test:
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include response_test.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o response_test

test: config_parser_test server_config_test response_test
	./config_parser_test
	./server_config_test
	./response_test
	python integration_test.py

clean: 
	rm -f $(TARGET) *.o *.a request_handler_test config_parser_test server_test server_config_test response_test *.gcno *.gcda