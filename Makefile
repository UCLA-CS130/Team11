# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -std=c++0x -g -Wall
LIBFLAGS = -lpthread -lboost_system
DEPS = config_parser.h serve_server.h server_response.h
OBJ = config_parser.o serve_main.o serve_server.o serve_response.o
TARGET = serve

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFLAGS)

clean: 
	rm -f $(TARGET) *.o 