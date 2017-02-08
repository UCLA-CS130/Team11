# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++11 -g -Wall
TCFLAGS = -g -fprofile-arcs -ftest-coverage
LIBFLAGS =  -lboost_system -lboost_filesystem -lpthread
SRC = config_parser.cc serve_main.cc server.cc request_handler.cc server_config.cc
GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock
TARGET= serve
TEST=config_parser_test
TEST_SRC= config_parser.cc


$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(TCFLAGS) $(LIBFLAGS) -o $(TARGET) 

config_parser_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	$(CC) -std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -I${GTEST_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc
	ar -rv libgtest.a gtest-all.o
	ar -rv libgmock.a gmock-all.o
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include $(TEST).cc $(TEST_SRC) ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o $(TEST)

# Remake unit tests for Serve 2.0
server_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include server_test.cc server.cc server_config.cc config_parser.cc request_handler.cc ${GMOCK_DIR}/src/gmock_main.cc libgtest.a libgmock.a $(LIBFLAGS) -o server_test

request_handler_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include request_handler_test.cc server_config.cc config_parser.cc request_handler.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o request_handler_test

server_config_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include server_config_test.cc server.cc config_parser.cc request_handler.cc server_config.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o server_config_test

test: config_parser_test server_config_test request_handler_test
	./config_parser_test
	./request_handler_test
	./server_config_test
	python integration_test.py

clean: 
	rm -f $(TARGET) *.o *.a request_handler_test config_parser_test server_test server_config_test *.gcno *.gcda
	lsof -P | grep ':9999' | awk '{print $2}' | xargs kill -9
