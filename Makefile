# chatclient make file
CC=clang
CCFLAGS=-Wall -Wextra -g
SOURCEDIR=src
EXECNAME=telnetchat

all: $(SOURCEDIR)/*.c
	$(CC) $(CCFLAGS) -o $(EXECNAME) $?
