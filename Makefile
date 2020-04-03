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
# CFLAGS += -O3
CFLAGS += -g
CFLAGS += -DDEBUG

SRC = $(wildcard $(SDIR)/*.cpp)
OBJ = ${SRC:.cpp=.o}
HDRS = $(wildcard $(IDIR)/*.h)

.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(SDIR)/%.o: $(SDIR)/%.cpp $(HDRS) Makefile
	@echo "$@, $<"
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ) *coredump*
