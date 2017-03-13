GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock
TEST_DIR=test
SRC_DIR=src

CC =g++
CFLAGS =-g -Wall -std=c++11 
LIBFLAGS = -lmysqlcppconn-static -lmysqlclient -ldl -lz -lc -static-libgcc -static-libstdc++ -pthread -Wl,-Bstatic -lboost_system -lboost_log_setup -lboost_log -lboost_filesystem -lboost_thread
TFLAGS =-std=c++11 -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -DBOOST_LOG_DYN_LINK
TARGS =-pthread
TLINK =-L./ -lmysqlcppconn -lmysqlclient -ldl -lz -lc -lgmock -lgtest -lboost_system -lboost_log -lboost_filesystem -lpthread
TARGET=serve

CCFILE = src/*.cc
HEADERS = src/*.h

$(TARGET): $(CCFILE) $(HEADERS)
	$(CC) $(CCFILE) $(CFLAGS) $(LIBFLAGS) -o $(TARGET)

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

request_handler_test: $(TEST_DIR)/request_handler_test.cc $(SRC_DIR)/request_handler.cc $(SRC_DIR)/request.cc $(SRC_DIR)/response.cc  $(SRC_DIR)/server.cc  $(SRC_DIR)/config_parser.cc  $(SRC_DIR)/server_config.cc $(SRC_DIR)/status_count.cc
	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o request_handler_test

request_test: $(TEST_DIR)/request_test.cc $(SRC_DIR)/request.cc
	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o request_test

response_test: $(TEST_DIR)/response_test.cc $(SRC_DIR)/response.cc
	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o response_test

server_config_test: $(TEST_DIR)/server_config_test.cc $(SRC_DIR)/server_config.cc $(SRC_DIR)/config_parser.cc $(SRC_DIR)/status_count.cc $(SRC_DIR)/config_parser.cc $(SRC_DIR)/request_handler.cc $(SRC_DIR)/response.cc
	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o server_config_test	

status_count_test: $(TEST_DIR)/status_count_test.cc $(SRC_DIR)/status_count.cc  $(SRC_DIR)/config_parser.cc $(SRC_DIR)/request_handler.cc $(SRC_DIR)/response.cc $(SRC_DIR)/server_config.cc $(SRC_DIR)/config_parser.cc
	$(CC) $(TFLAGS) $(TARGS) $^ ${GTEST_DIR}/src/gtest_main.cc $(TLINK) -o status_count_test	

test: gtest_setup config_parser_test request_handler_test request_test response_test server_config_test status_count_test
	./config_parser_test
	./request_handler_test
	./request_test
	./response_test
	./server_config_test
	./status_count_test
	python $(TEST_DIR)/integration_test.py
	

# Create the initial build file 
docker: Dockerfile
	docker build -t serve.build .
	docker run --rm serve.build > serve.tar

deploy: serve.tar Dockerfile.run
	rm -rf deploy
	mkdir deploy
	tar -xf serve.tar -C deploy
	cp Dockerfile.run deploy
	cp new_config deploy
	cp -r example_dir deploy
	cp -r example_dir_2 deploy
	cd deploy; \
	docker build -f Dockerfile.run -t serve.deploy .
	chmod 400 team11-ec2-key-pair.pem
	docker save serve.deploy | bzip2 | ssh -i "team11-ec2-key-pair.pem" ec2-user@ec2-52-26-164-101.us-west-2.compute.amazonaws.com 'bunzip2 | docker load'
	ssh -i "team11-ec2-key-pair.pem" ec2-user@ec2-52-26-164-101.us-west-2.compute.amazonaws.com -t 'docker stop $$(docker ps -a -q); docker run -d -t -p 80:8011 serve.deploy;'

clean:
	rm -rf $(TARGET) config_parser_test request_handler_test request_test response_test server_config_test status_count_test libgmock.a libgtest.a deploy/ serve.tar serve.dSYM

.PHONY: clean test deploy


