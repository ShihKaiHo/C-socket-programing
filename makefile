.PHONY: clean all

flag = -std=c++11 -O2
CC = g++

all: client server

client: client.cpp
	$(CC) -o $@ $< $(flag)

server: server.cpp
	$(CC) -o $@ $< $(flag)

clean:
	rm -f server client *.o