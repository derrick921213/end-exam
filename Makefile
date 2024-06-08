.DEFAULT_GOAL := all
CC = gcc
CFLAGS = -Wall -Wextra -I./include
ifdef OS
    ifeq ($(OS),Windows_NT)
        SHELL := C:\Windows\System32\cmd.exe
        MKDIR_P = if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
        RM = del
        PWD = cd
        EXEC_EXT = .exe
        PATHSEP = /
        PATHTRANSFORM = $(subst /,$(PATHSEP),$(1))
    else
        SHELL := /bin/bash
        MKDIR_P = mkdir -p $1
        RM = rm -f
        PWD = pwd
        EXEC_EXT = 
        PATHSEP = /
        PATHTRANSFORM = $(1)
    endif
else
    SHELL := /bin/bash
    MKDIR_P = mkdir -p $1
    RM = rm -f
    PWD = pwd
    EXEC_EXT = 
    PATHSEP = /
    PATHTRANSFORM = $(1)
endif
WORK_DIR = $(shell $(PWD))
SRC = $(call PATHTRANSFORM,$(WORK_DIR)/src)
BIN = $(call PATHTRANSFORM,$(WORK_DIR)/bin)
OBJ = $(call PATHTRANSFORM,$(WORK_DIR)/obj)
SOURCES = $(wildcard $(SRC)$(PATHSEP)*.c)
OBJECTS = $(patsubst $(SRC)$(PATHSEP)%.c, $(OBJ)$(PATHSEP)%.o, $(SOURCES))
EXECUTABLE = $(BIN)$(PATHSEP)main$(EXEC_EXT)
.PHONY: all clean compile run directories print-vars debug
all: directories $(EXECUTABLE)
	@$(EXECUTABLE) $(WORK_DIR)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
$(OBJ)$(PATHSEP)%.o: $(SRC)$(PATHSEP)%.c | directories
	$(CC) $(CFLAGS) -c -o $@ $<
directories:
	$(call MKDIR_P,$(BIN))
	$(call MKDIR_P,$(OBJ))
compile: directories
ifndef TARGET
	$(error TARGET is undefined. Usage: make compile TARGET=<filename>)
endif
	$(CC) $(CFLAGS) -o $(BIN)$(PATHSEP)$(TARGET)$(EXEC_EXT) $(SRC)$(PATHSEP)$(TARGET).c
run: compile
	$(BIN)$(PATHSEP)$(TARGET)$(EXEC_EXT)
clean:
	$(RM) $(BIN)$(PATHSEP)*$(EXEC_EXT) $(OBJ)$(PATHSEP)*.o
debug:
	$(info WORK_DIR = $(WORK_DIR))
	$(info SRC = $(SRC))
	$(info BIN = $(BIN))
	$(info OBJ = $(OBJ))
	$(info WILDCARD SOURCES = $(wildcard $(SRC)$(PATHSEP)*.c))
	$(info PATSUBST OBJECTS = $(patsubst $(SRC)$(PATHSEP)%.c, $(OBJ)$(PATHSEP)%.o, $(wildcard $(SRC)$(PATHSEP)*.c)))
	@true
print-vars:
	$(info SHELL = $(SHELL))
	$(info MKDIR_P = $(MKDIR_P))
	$(info RM = $(RM))
	$(info PWD = $(PWD))
	$(info CC = $(CC))
	$(info CFLAGS = $(CFLAGS))
	$(info EXEC_EXT = $(EXEC_EXT))
	$(info PATHSEP = $(PATHSEP))
	$(info WORK_DIR = $(WORK_DIR))
	$(info SRC = $(SRC))
	$(info BIN = $(BIN))
	$(info OBJ = $(OBJ))
	$(info SOURCES = $(SOURCES))
	$(info OBJECTS = $(OBJECTS))
	$(info EXECUTABLE = $(EXECUTABLE))