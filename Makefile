# CC = gcc
# CFLAGS = -Wall -Wextra -I.
# SRC = src
# BIN = bin
# OBJ = obj

# SOURCES = $(wildcard $(SRC)/*.c)
# EXECUTABLES = $(patsubst $(SRC)/%.c, $(BIN)/%, $(SOURCES))

# all: $(EXECUTABLES)

# $(BIN)/%: $(SRC)/%.c
# 	@mkdir -p $(BIN)
# 	@mkdir -p $(OBJ)
# 	$(CC) $(CFLAGS) -o $@ $<

# .PHONY: clean

# clean:
# 	@rm -f $(BIN)/* $(OBJ)/*

# CC = gcc
# CFLAGS = -Wall -Wextra -I.
# SRC = src
# BIN = bin
# OBJ = obj

# SOURCES = $(wildcard $(SRC)/*.c)
# EXECUTABLES = $(patsubst $(SRC)/%.c, $(BIN)/%, $(SOURCES))

# .PHONY: all clean compile

# all: $(EXECUTABLES)

# $(BIN)/%: $(SRC)/%.c
# 	@mkdir -p $(BIN)
# 	@mkdir -p $(OBJ)
# 	$(CC) $(CFLAGS) -o $@ $<

# # 編譯指定的目標文件
# compile: 
# ifndef TARGET
# 	$(error TARGET is undefined. Usage: make compile TARGET=<filename>)
# endif
# 	@mkdir -p $(BIN)
# 	@mkdir -p $(OBJ)
# 	$(CC) $(CFLAGS) -o $(BIN)/$(TARGET) $(SRC)/$(TARGET).c

# clean:
# 	@rm -f $(BIN)/* $(OBJ)/*


CC = gcc
CFLAGS = -Wall -Wextra -I.
SRC = src
BIN = bin
OBJ = obj

SOURCES = $(wildcard $(SRC)/*.c)
EXECUTABLES = $(patsubst $(SRC)/%.c, $(BIN)/%, $(SOURCES))

.PHONY: all clean compile run

all: $(EXECUTABLES)

$(BIN)/%: $(SRC)/%.c
	@mkdir -p $(BIN)
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

# 編譯指定的目標文件
compile: 
ifndef TARGET
	$(error TARGET is undefined. Usage: make compile TARGET=<filename>)
endif
	@mkdir -p $(BIN)
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN)/$(TARGET) $(SRC)/$(TARGET).c

# 編譯並運行指定的目標文件
run: compile
	$(BIN)/$(TARGET)

clean:
	rm -f $(BIN)/* $(OBJ)/*
