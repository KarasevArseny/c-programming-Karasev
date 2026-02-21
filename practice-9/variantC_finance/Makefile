CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -I include
CFLAGS_DEBUG = -g -O0
CFLAGS_RELEASE = -O2 -DNDEBUG
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = data

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/finance_manager

.PHONY: all clean debug release test data_dir

all: CFLAGS = $(CFLAGS_DEBUG)
all: data_dir $(TARGET)

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: data_dir $(TARGET)

release: CFLAGS = $(CFLAGS_RELEASE)
release: data_dir $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

data_dir:
	mkdir -p $(DATA_DIR)

test: CFLAGS = $(CFLAGS_DEBUG)
test: $(BIN_DIR)/test_finance
	./$(BIN_DIR)/test_finance

$(BIN_DIR)/test_finance: tests/test_finance.c $(filter-out $(OBJ_DIR)/main.o,$(OBJS)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run