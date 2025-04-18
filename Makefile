SFML_PREFIX := lib/SFML/installed

CC   := gcc
CXX  := g++

CFLAGS   := -Wall -Wextra -std=c11   -ggdb -Iinclude
CXXFLAGS := -Wall -Wextra -std=c++17 -ggdb -DSFML_STATIC \
            -I$(SFML_PREFIX)/include -Iinclude

LDFLAGS  := -L$(SFML_PREFIX)/lib \
            -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s \
            -lFLAC -lvorbisenc -lvorbisfile -lvorbis -logg \
            -lsndfile -lopenal \
            -lX11 -lXrandr -lXcursor -lXi -lGL -lpthread -ldl -ludev

SRC_DIR := src
OBJ_DIR := bin
SRC_C   := $(shell find $(SRC_DIR) -name '*.c')
SRC_CPP := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_C   := $(patsubst $(SRC_DIR)/%.c,  $(OBJ_DIR)/%.o,$(SRC_C))
OBJ_CPP := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_CPP))
OBJ     := $(OBJ_C) $(OBJ_CPP)

TARGET  := $(OBJ_DIR)/labirinto_quantico

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC)  $(CFLAGS)  -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $^ $(LDFLAGS) -o $@
	@echo "Executável gerado em $@"

run: $(TARGET)
	@./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)

.PHONY: all run clean
