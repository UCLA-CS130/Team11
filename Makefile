# Makefile based of tutorial here:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -g -Wall -std=c++0x
PARSER = nginx-configparser
DEPS = config_parser.h
OBJ = config_parser.o main.o 
TARGET = serve

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

serve: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean: 
	rm -f $(TARGET) *.o 