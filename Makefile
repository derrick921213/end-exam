CC = gcc
CFLAGS = -Wall -Wextra -I./include
SRC = src
BIN = bin
OBJ = obj

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
EXECUTABLE = $(BIN)/main

.PHONY: all clean compile run

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 編譯指定的目標文件
compile:
ifndef TARGET
	$(error TARGET is undefined. Usage: make compile TARGET=<filename>)
endif
	$(CC) $(CFLAGS) -o $(BIN)/$(TARGET) $(SRC)/$(TARGET).c

# 編譯並運行指定的目標文件
run: compile
	$(BIN)/$(TARGET)

clean:
	@rm -f $(BIN)/* $(OBJ)/*
ifeq ($(TARGET), main)
all: $(EXECUTABLE)
	@$(EXECUTABLE)
endif
