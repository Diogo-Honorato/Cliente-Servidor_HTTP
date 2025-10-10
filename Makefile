COMPILADORC = gcc
COMPILADOR_FLAGS = -Wall -Wextra -O2 -g
SANITIZE_FLAGS = -fsanitize=address
LDFLAGS = -pthread
EXT = .c

SRC_DIR = source
HEADER_DIRS = include
BIN_DIR = bin
OBJ_DIR = objects
TARGET = main

INCLUDES = $(foreach dir, $(HEADER_DIRS), -I$(dir))

SRCS = $(shell find $(SRC_DIR) -type f -name '*$(EXT)')
OBJS = $(patsubst $(SRC_DIR)/%$(EXT), $(OBJ_DIR)/%.o, $(SRCS))


ifeq ($(SANITIZE), 1)
    COMPILADOR_FLAGS += $(SANITIZE_FLAGS)
endif

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	@$(COMPILADORC) $(COMPILADOR_FLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%$(EXT)
	@mkdir -p $(@D)
	@echo $<
	@$(COMPILADORC) $(COMPILADOR_FLAGS) $(INCLUDES) -c $< -o $@

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


valgrind:
	@valgrind --leak-check=full $(BIN_DIR)/$(TARGET) $(ARGS) 

sanitize:
	@$(MAKE) SANITIZE=1

clean:
	@rm -rf $(BIN_DIR) $(OBJ_DIR)

run: all
	@$(BIN_DIR)/$(TARGET) $(ARGS)


.PRECIOUS: $(OBJS)
.PHONY: all valgrind sanitize clean run