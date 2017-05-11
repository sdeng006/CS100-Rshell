CC= g++

CFLAGS = -Wall -Werror -ansi -pedantic

all:
	mkdir -p ./bin
	$(CC) $(CFLAGS) ./src/main.cpp -o ./bin/rshell

main:
	$(CC) $(CFLAGS) ./src/main.cpp

Connector:
	$(CC) $(CFLAGS) ./src/Connector.cpp

Cmd:
	$(CC) $(CFLAGS) ./src/Cmd.cpp

And:
	$(CC) $(CFLAGS) ./src/And.cpp

Or:
	$(CC) $(CFLAGS) ./src/Or.cpp

Semicolon:
	$(CC) $(CFLAGS) ./src/Semicolon.cpp

clean:
	rm -rf ./bin