# Compiler and compiler flags
CC = gcc
CFLAGS = -g -Wall -I./bibliotecas

# Directories
SRC_DIRS = auxiliares auxiliares_arvB funcionalidades funcionalidades_arvB
INCLUDE_DIR = bibliotecas

# Source files
SRCS = $(wildcard $(foreach dir, $(SRC_DIRS), $(dir)/*.c)) main.c

# Target executable
TARGET = programaTrab

# Default rule
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Run the program
run: all
	./$(TARGET)

# Run with Valgrind
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET) valgrind.log
