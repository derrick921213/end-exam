# CC = gcc
# CFLAGS = -Wall -Wextra -I./include
# WORK_DIR = $(shell pwd)
# SRC = $(WORK_DIR)/src
# BIN = $(WORK_DIR)/bin
# OBJ = $(WORK_DIR)/obj

# SOURCES = $(wildcard $(SRC)/*.c)
# OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
# EXECUTABLE = $(BIN)/main

# .PHONY: all clean compile run

# all: $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $@ $^

# $(OBJ)/%.o: $(SRC)/%.c
# 	$(CC) $(CFLAGS) -c -o $@ $<

# # 編譯指定的目標文件
# compile:
# ifndef TARGET
# 	$(error TARGET is undefined. Usage: make compile TARGET=<filename>)
# endif
# 	$(CC) $(CFLAGS) -o $(BIN)/$(TARGET) $(SRC)/$(TARGET).c

# # 編譯並運行指定的目標文件
# run: compile
# 	$(BIN)/$(TARGET)

# clean:
# 	@rm -f $(BIN)/* $(OBJ)/*
# ifeq ($(TARGET), main)
# all: $(EXECUTABLE)
# 	@$(EXECUTABLE) $(WORK_DIR)
# endif

# Detect platform
ifdef OS
    ifeq ($(OS),Windows_NT)
        MKDIR_P = if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
        RM = del
        CC = gcc
        CFLAGS = -Wall -Wextra -I./include
        EXEC_EXT = .exe
        PATHSEP = \\
    else
        MKDIR_P = mkdir -p $1
        RM = rm -f
        CC = gcc
        CFLAGS = -Wall -Wextra -I./include
        EXEC_EXT = 
        PATHSEP = /
    endif
else
    MKDIR_P = mkdir -p $1
    RM = rm -f
    CC = gcc
    CFLAGS = -Wall -Wextra -I./include
    EXEC_EXT = 
    PATHSEP = /
endif

WORK_DIR = $(shell pwd)
SRC = $(WORK_DIR)$(PATHSEP)src
BIN = $(WORK_DIR)$(PATHSEP)bin
OBJ = $(WORK_DIR)$(PATHSEP)obj

SOURCES = $(wildcard $(SRC)$(PATHSEP)*.c)
OBJECTS = $(patsubst $(SRC)$(PATHSEP)%.c, $(OBJ)$(PATHSEP)%.o, $(SOURCES))
EXECUTABLE = $(BIN)$(PATHSEP)main$(EXEC_EXT)

.PHONY: all clean compile run directories

all: directories $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)$(PATHSEP)%.o: $(SRC)$(PATHSEP)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

directories:
	$(call MKDIR_P,$(BIN))
	$(call MKDIR_P,$(OBJ))

# Compile a specific target file
compile:
ifndef TARGET
	$(error TARGET is undefined. Usage: make compile TARGET=<filename>)
endif
	$(CC) $(CFLAGS) -o $(BIN)$(PATHSEP)$(TARGET)$(EXEC_EXT) $(SRC)$(PATHSEP)$(TARGET).c

# Compile and run a specific target file
run: compile
	$(BIN)$(PATHSEP)$(TARGET)$(EXEC_EXT)

clean:
	$(RM) $(BIN)$(PATHSEP)*$(EXEC_EXT) $(OBJ)$(PATHSEP)*.o

ifeq ($(TARGET), main)
all: directories $(EXECUTABLE)
	@$(EXECUTABLE) $(WORK_DIR)
endif