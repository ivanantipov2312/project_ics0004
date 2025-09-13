BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src

CC=gcc
LD=gcc
CFLAGS=-O2 -Wall -Wextra -I$(SRC_DIR)

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(BIN_DIR)/app

$(BIN_DIR)/app: $(BIN_DIR) $(OBJ_DIR) $(OBJS)
	$(LD) $(OBJS) -o $@

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean remake run

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

remake: clean $(BIN_DIR)/app

run: $(BIN_DIR)/app
	./$(BIN_DIR)/app
