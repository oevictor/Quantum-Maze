# --- Makefile (handles .c, .cpp, and headers) ---

# Compilers
CC          := gcc
CXX         := g++
# Flags for C (e.g., -std=c11) and C++ (e.g., -std=c++17)
CFLAGS      := -Wall -Wextra -std=c11 -ggdb -Iinclude
CXXFLAGS    := -Wall -Wextra -std=c++17 -ggdb -Iinclude -Ilib/SFML/installed/include

# SFML linking flags (for C++ binary)
SFML_LIBDIR := -Llib/SFML/installed/lib
SFML_LIBS   := \
  -lsfml-graphics-s -lsfml-window-s -lsfml-system-s \
  -lX11 -lXrandr -lXcursor -lXi -lGL \
  -lpthread -ldl -ludev

# Find all C/C++ sources recursively in src/
SRC_C       := $(shell find src -type f -name '*.c')
SRC_CPP     := $(shell find src -type f -name '*.cpp')
# Generate corresponding .o files in bin/
OBJ_C       := $(patsubst src/%.c,bin/%.o,$(SRC_C))
OBJ_CPP     := $(patsubst src/%.cpp,bin/%.o,$(SRC_CPP))
OBJ         := $(OBJ_C) $(OBJ_CPP)
TARGET      := bin/labirinto_quantico

# Default: build the executable
all: $(TARGET)

# Compile C sources → object files
bin/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ sources → object files
bin/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link all objects → binary (using g++ for C++ dependencies like SFML)
$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) $^ $(SFML_LIBDIR) $(SFML_LIBS) -o $@

# Build then run
run: $(TARGET)
	@echo "Running $(TARGET)…"
	@./$(TARGET)

# Clean all binaries
clean:
	rm -rf bin

.PHONY: all run clean