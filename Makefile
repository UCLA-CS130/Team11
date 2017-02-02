# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++0x -g -Wall
TCFLAGS = -g -fprofile-arcs -ftest-coverage
LIBFLAGS = -lboost_system
SRC = config_parser.cc serve_main.cc serve_server.cc serve_response.cc
TARGET = serve

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(TCFLAGS) $(LIBFLAGS) -o $(TARGET) 

clean: 
	rm -f $(TARGET) *.o 

test:
	python integration_test.py

