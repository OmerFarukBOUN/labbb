CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lcudd -ldd -lmtr -lst -lutil -lpthread  # Link CUDD and dependencies
INCLUDES = -I/usr/local/include  # Path to CUDD headers (adjust if necessary)
LIBS = -L/usr/local/lib          # Path to CUDD libraries (adjust if necessary)

all: cudd_test

cudd_test: cudd_test.c
	$(CC) $(CFLAGS) $(INCLUDES) cudd_test.c -o cudd_test $(LIBS) $(LDFLAGS)

clean:
	rm -f cudd_test
