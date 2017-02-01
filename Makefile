# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++0x -g -Wall
LIBFLAGS = -lboost_system -lpthread
SRC = config_parser.cc serve_main.cc serve_server.cc serve_response.cc
GTEST_DIR=googletest/googletest
TARGET= serve
TEST=config_parser_test
TEST_SRC= config_parser.cc

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBFLAGS) -o $(TARGET)

config_parser_test:
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CC) -std=c++0x -isystem ${GTEST_DIR}/include $(TEST).cc $(TEST_SRC) ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(LIBFLAGS) -o $(TEST)

test: $(TEST)
	./$(TEST)

clean: 
	rm -f $(TARGET) *.o 