CC=gcc
CFLAGS=-std=c17
LDFLAGS=
PROJECT_NAME=run

build: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_NAME) main.c

clean:
	rm -f $(PROJECT_NAME)
