CC = gcc
CFLAGS = -Wall $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lm
DEBUGFLAGS = -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DEBUG_OBJ_DIR = $(OBJ_DIR)/debug
DEBUG_BIN_DIR = $(BIN_DIR)/debug

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEBUG_OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(DEBUG_OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/pong
DEBUG_TARGET = $(DEBUG_BIN_DIR)/pong

# Normal build
all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += $(DEBUGFLAGS)
debug: LDFLAGS += $(DEBUGFLAGS)
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJECTS) | $(DEBUG_BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DEBUG_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) $(DEBUG_BIN_DIR) $(DEBUG_OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(OBJ_DIR) $(BIN_DIR) $(DEBUG_OBJ_DIR) $(DEBUG_BIN_DIR)

run: $(TARGET)
	@./$(TARGET)

debug_run: debug
	@./$(DEBUG_TARGET)

.PHONY: all clean run debug debug_run
