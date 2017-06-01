CC= g++

CC_FLAGS = -std=c++11 -Wall -Werror -ansi -pedantic

all:
	mkdir -p ./bin
	$(CC) $(CC_FLAGS) ./src/main.cpp -o ./bin/rshell

rshell:
	$(CC) $(CC_FLAGS) ./bin/rshell

clean:
	rm -rf ./bin
