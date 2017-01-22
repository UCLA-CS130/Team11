OPTIMIZE=-O0
CC = g++
CFLAGS = $(OPTIMIZE) -g -Wall -std=c++0x
PARSER = nginx-configparser
DEPS = $(PARSER)/config_parser.h
OBJ = $(PARSER)/config_parser.o main.o 

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

webserver: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean: 
	rm -f webserver *.o 
	rm -f $(PARSER)/*.o