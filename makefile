#CC=g++
#CFLAGS=-O3 -Wfatal-errors -Wall -Wextra
#MFLAGS=-O3 -Wfatal-errors

CC=gcc
CFLAGS=-std=c99 -O3 -Wfatal-errors -Wall -Wextra
MFLAGS=-std=c99 -O3 -Wfatal-errors

SRC_F=code
BIN_F=bin

SRC_C=$(wildcard $(SRC_F)/*.c)
DEP_C=$(wildcard $(SRC_F)/*.h)
OBJ_C=$(patsubst $(SRC_F)/%.c, $(BIN_F)/%.o, $(SRC_C))

LNK=-Llib

lGLFW=-lglfw3
lGLEW=-lglew32
lGL=-lopengl32

run: all
	@echo === Running Repository =================================================
	@$(BIN_F)/yMain.exe

build: all
	@echo === Building Repository ================================================

all: $(BIN_F)/yMain.exe

$(BIN_F)/yMain.exe: $(BIN_F)/main.o $(OBJ_C)
	$(CC) $(CFLAGS) -o $@ $^ $(LNK) $(lGLEW) $(lGLFW) $(lGL)

$(BIN_F)/main.o: main.c $(DEP_C)
	$(CC) $(MFLAGS) -c $< -o $@

$(BIN_F)/%.o: $(SRC_F)/%.c $(SRC_F)/%.h
	$(CC) $(CFLAGS) -c $< -o $@