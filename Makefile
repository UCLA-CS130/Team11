GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock
TEST_DIR=test
SRC_DIR=src
BUILD_DIR=build

CC =g++
CFLAGS =-g -Wall -std=c++11 
LIBFLAGS =-static-libgcc -static-libstdc++ -pthread -Wl,-Bstatic -lboost_system -lboost_log_setup -lboost_log -lboost_filesystem -lboost_thread
TFLAGS =-std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -DBOOST_LOG_DYN_LINK
TARGS =-pthread
TLINK =-L./build/ -lgmock -lgtest -lboost_system -lboost_log -lboost_filesystem -lpthread
TARGET=serve

CCFILE = src/*.cc
HEADERS = src/*.h

$(TARGET): $(CCFILE) $(HEADERS)
	$(CC) $(CCFILE) $(CFLAGS) $(LIBFLAGS) -o serve

gtest_setup:
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} \
	-pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} \
	-isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} \
	-pthread -c ${GMOCK_DIR}/src/gmock-all.cc
	ar -rv libgmock.a gtest-all.o gmock-all.o
	rm gtest-all.o gmock-all.o


config_parser_test: $(TEST_DIR)/config_parser_test.cc $(SRC_DIR)/config_parser.cc
	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o config_parser_test

#request_handler_test: $(TEST_DIR)/request_handler_test.cc $(SRC_DIR)/request_handler.cc $(SRC_DI#R)/request.cc $(SRC_DIR)/response.cc 
#	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o request_handler_test

#request_test: $(TEST_DIR)/request_test.cc $(SRC_DIR)/request.cc
#	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o request_test

#response_test: $(TEST_DIR)/response_test.cc $(SRC_DIR)/response.cc
#	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o response_test

#server_config_test: $(TEST_DIR)/server_config_test.cc $(SRC_DIR)/server_config.cc
#	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o server_config_test	

#status_count_test: $(TEST_DIR)/status_count_test.cc $(SRC_DIR)/status_count.cc
#	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o status_count_test	

test: gtest_setup config_parser_test
	./config_parser_test

clean:
	rm -rf $(TARGET) config_parser_test
