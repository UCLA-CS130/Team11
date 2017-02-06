# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++11 -g -Wall
TCFLAGS = -g -fprofile-arcs -ftest-coverage
LIBFLAGS =  -lboost_system -lboost_filesystem -lpthread
SRC = config_parser.cc serve_main.cc server.cc request_handler.cc server_config.cc
GTEST_DIR=googletest/googletest
TARGET= serve
TEST=config_parser_test
TEST_SRC= config_parser.cc


$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(TCFLAGS) $(LIBFLAGS) -o $(TARGET) 

config_parser_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include $(TEST).cc $(TEST_SRC) ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o $(TEST)

# Remake unit tests for Serve 2.0
#serve_server_test:
	#$(CC) -std=c++0x -isystem ${GTEST_DIR}/include serve_server_test.cc server.cc serve_response.cc config_parser.cc request_handler.cc server_config.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o serve_server_test

#serve_response_test:
	#$(CC) -std=c++0x -isystem ${GTEST_DIR}/include serve_response_test.cc server.cc serve_response.cc config_parser.cc request_handler.cc server_config.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o serve_response_test



test: config_parser_test
	./config_parser_test
	python integration_test.py

clean: 
	rm -f $(TARGET) *.o *.a serve_server_test config_parser_test serve_response_test *.gcno *.gcda
	lsof -P | grep ':9999' | awk '{print $2}' | xargs kill -9


