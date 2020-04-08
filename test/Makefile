# Makefile for chip8 project tests
#
# Travis Banken
# 2020

CC = g++
TARGET = chip8-tests

IDIR = ../include

CFLAGS = -std=c++14
CFLAGS += -Wall -Wextra
CFLAGS += -I$(IDIR)
# CFLAGS += -O3
CFLAGS += -g
CFLAGS += -DDEBUG

SRC = $(wildcard *.cpp)
OBJ = ${SRC:.cpp=.o}
EXTRA_OBJ = ../src/bus.o ../src/chip8.o ../src/mem.o
HDRS = $(wildcard *.h)
HDRS += $(wildcard $(IDIR)/*.h)

.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(EXTRA_OBJ) -o $(TARGET)

%.o: %.cpp $(HDRS) Makefile
	@echo "$@, $<"
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ) *coredump*
