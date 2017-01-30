# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++0x -g -Wall
LIBFLAGS = -lboost_system
SRC = config_parser.cc serve_main.cc serve_server.cc serve_response.cc
TARGET = serve

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBFLAGS) -o $(TARGET)

clean: 
	rm -f $(TARGET) *.o 

test:
	python3 integration_test.py