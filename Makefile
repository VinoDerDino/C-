CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude -I$(INC_DIR) $(SDL_INCLUDE_DIR)
LDFLAGS = $(SDL_LIB_DIR) $(SDLFLAGS)

SDL_INCLUDE_DIR = -IE:\programming\SDL\x86_64-w64-mingw32\include
SDL_LIB_DIR = -LE:\programming\SDL\x86_64-w64-mingw32\lib
SDLFLAGS = -lmingw32 -lSDL2main -lSDL2

SRC_DIR = src
INC_DIR = h_files
OBJ_DIR = obj

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET = TILE_GAME

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
