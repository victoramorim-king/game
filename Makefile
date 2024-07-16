# Compiler
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build

# Output
TARGET = $(BUILD_DIR)/GameEngine.exe

# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic

# Linker flags
LDFLAGS = -lgdi32

# Build target
all: create_build_dir $(TARGET)

# Create build directory if it doesn't exist
create_build_dir:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Link
$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean build directory
clean:
	del /Q $(BUILD_DIR)\*

# Run the program
run: $(TARGET)
	$(TARGET)

# Phony targets
.PHONY: all clean run create_build_dir
