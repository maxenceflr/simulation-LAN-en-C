# filepath: /home/maxime/BUT_S2/S21/SAES21/Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -pedantic

# Source files
SRCS = main.c graphe.c reseau.c initReseau.c stp.c

# Header files
HEADERS = graphe.h reseau.h initReseau.h stp.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = main

# Default target
all: $(EXEC)

# Link the object files to create the executable
$(EXEC): $(OBJS)
	$(CC) -o $@ $^

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean