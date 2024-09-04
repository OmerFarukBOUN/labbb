# Compiler
CC = gcc

# Compiler flags
CFLAGS = -I/usr/local/include -I/usr/include

# Linker flags
LDFLAGS = -L/usr/local/lib -lcudd -lm

# Source files
SRCS = cudd_test.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
EXEC = cudd_test

# Build the executable
$(EXEC): $(OBJS)
    $(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Compile the source files
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
    rm -f $(OBJS) $(EXEC)
