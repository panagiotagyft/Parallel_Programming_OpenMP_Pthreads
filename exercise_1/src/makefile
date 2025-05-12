# Compiler
CC = gcc

# Paths
INCLUDE = ../include
MODULES = ../modules
BUILD = ../build

# Compile options
CFLAGS = -Wall -Werror -Wno-error=sign-compare -Wextra -g3 -I$(INCLUDE) -fopenmp
LDFLAGS = -lm -lgomp -lpthread

# Create the build directory if it doesn't exist
$(shell mkdir -p $(BUILD))

# .o files needed
MODS := $(wildcard $(MODULES)/*.c)
OBJS := $(patsubst $(MODULES)/%.c,$(BUILD)/%.o, $(MODS)) $(BUILD)/main.o

# Program
PROGRAM = my_program

# Command line arguments -->> make run
ARGS =

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(LDFLAGS)

$(BUILD)/%.o: $(MODULES)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD)/main.o: main.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) $(PROGRAM)
	rm -rf $(BUILD)

valgrind: $(PROGRAM)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(PROGRAM) $(ARGS)

run: $(PROGRAM)
	./$(PROGRAM) $(ARGS)