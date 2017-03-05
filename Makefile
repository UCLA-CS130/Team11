# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++11 -g -Wall -DBOOST_LOG_DYN_LINK
TCFLAGS = --coverage
LIBFLAGS =  -lboost_system -lboost_filesystem -lboost_log -lboost_thread -lpthread
SRC_DIR=src
TEST_DIR=test
SRC_FILES=$(wildcard src/*.cc)
SRC = config_parser.cc serve_main.cc server.cc request_handler.cc server_config.cc request.cc response.cc
GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock
TARGET=serve

$(TARGET): $(SRC_FILES)
	$(CC) $(SRC_FILES) $(CFLAGS) $(LIBFLAGS) -o $(TARGET) 

config_parser_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	$(CC) -std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -I${GTEST_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc
	ar -rv libgtest.a gtest-all.o
	ar -rv libgmock.a gmock-all.o
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include -I${SRC_DIR} $(TEST_DIR)/config_parser_test.cc $(SRC_DIR)/config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o config_parser_test

server_test:
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -I${SRC_DIR} $(TEST_DIR)/server_test.cc $(SRC_DIR)/server.cc $(SRC_DIR)/server_config.cc $(SRC_DIR)/status_count.cc $(SRC_DIR)/config_parser.cc $(SRC_DIR)/request_handler.cc $(SRC_DIR)/response.cc $(SRC_DIR)/request.cc ${GMOCK_DIR}/src/gmock_main.cc libgtest.a libgmock.a $(LIBFLAGS) -o server_test

request_handler_test:
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include -I${SRC_DIR} $(TEST_DIR)/request_handler_test.cc $(SRC_DIR)/server.cc $(SRC_DIR)/config_parser.cc $(SRC_DIR)/status_count.cc $(SRC_DIR)/request_handler.cc $(SRC_DIR)/server_config.cc $(SRC_DIR)/response.cc $(SRC_DIR)/request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o request_handler_test

server_config_test:
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include -I${SRC_DIR} $(TEST_DIR)/server_config_test.cc $(SRC_DIR)/server.cc $(SRC_DIR)/config_parser.cc $(SRC_DIR)/status_count.cc $(SRC_DIR)/request_handler.cc $(SRC_DIR)/server_config.cc $(SRC_DIR)/response.cc $(SRC_DIR)/request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o server_config_test

response_test:
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include -I${SRC_DIR} $(TEST_DIR)/response_test.cc $(SRC_DIR)/server_config.cc $(SRC_DIR)/config_parser.cc $(SRC_DIR)/status_count.cc $(SRC_DIR)/request.cc $(SRC_DIR)/request_handler.cc $(SRC_DIR)/response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o response_test

request_test:
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include -I${SRC_DIR} $(TEST_DIR)/request_test.cc $(SRC_DIR)/server_config.cc $(SRC_DIR)/config_parser.cc $(SRC_DIR)/status_count.cc $(SRC_DIR)/request.cc $(SRC_DIR)/request_handler.cc $(SRC_DIR)/response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o request_test

status_count_test:
	$(CC) $(CFLAGS) -std=c++0x -isystem ${GTEST_DIR}/include -I${SRC_DIR} $(TEST_DIR)/status_count_test.cc $(SRC_DIR)/server_config.cc $(SRC_DIR)/status_count.cc $(SRC_DIR)/response.cc $(SRC_DIR)/request.cc $(SRC_DIR)/config_parser.cc $(SRC_DIR)/request_handler.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o status_count_test

# NOTE: config_parser_test must be executed first -- the command does the googletest setup neccessary for the later tests

test: config_parser_test server_test request_handler_test server_config_test response_test request_test status_count_test
	./config_parser_test
	./server_test
	./request_handler_test
	./server_config_test
	./response_test
	./request_test
	./status_count_test
	python $(TEST_DIR)/integration_test.py
	python $(TEST_DIR)/multithreading_test.py 4

clean: 
	rm -f $(TARGET) *.o *.a request_handler_test config_parser_test server_test server_config_test response_test status_count_test request_test *.gcno *.gcda