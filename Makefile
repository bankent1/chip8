# Makefile for chip8 project
#
# Travis Banken
# 2020

CC = g++
TARGET = chip8

SDIR = src
IDIR = include

CFLAGS = -std=c++14
CFLAGS += -I$(IDIR)
CFLAGS += -Wall -Wextra
CFLAGS += $(shell sdl2-config --cflags)
# CFLAGS += -O3
CFLAGS += -g
CFLAGS += -DDEBUG

LIBS = $(shell sdl2-config --libs)

SRC = $(wildcard $(SDIR)/*.cpp)
OBJ = ${SRC:.cpp=.o}
HDRS = $(wildcard $(IDIR)/*.h)

.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(TARGET)

$(SDIR)/%.o: $(SDIR)/%.cpp $(HDRS) Makefile
	@echo "$@, $<"
	$(CC) $(CFLAGS) -c $< $(LIBS) -o $@

.PHONY: tests
tests: build
	@make -C test

.PHONY: run-tests
run-tests: tests
	./test/chip8-tests

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ) *dump* *.log
	@make -C test clean
