all:
	gcc -Wall -c common.c
	gcc -Wall client.c common.o -o bin/client
	gcc -Wall server-mt.c  common.o -o bin/server -lm