# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++0x -g -Wall
TCFLAGS = -g -fprofile-arcs -ftest-coverage
LIBFLAGS = -lboost_system -lpthread
SRC = config_parser.cc serve_main.cc server.cc serve_response.cc
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

serve_server_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include serve_server_test.cc serve_server.cc serve_response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o serve_server_test

serve_response_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include serve_response_test.cc serve_server.cc serve_response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o serve_response_test



test: config_parser_test serve_server_test serve_response_test
	./serve_response_test
	./serve_server_test
	python integration_test.py

clean: 
	rm -f $(TARGET) *.o serve_server_test config_parser_test serve_response_test *.gcno *.gcda


