TARGET = chip8
INCLUDE = ./include

CC = gcc

CFLAGS = -Wall -I$(INCLUDE)
CFLAGS += -g # debug
CFLAGS += -DDEBUG # output debug msg
# CFLAGS += -O3 # release optimizations

SDIR = ./src
TDIR = $(SDIR)/tests

CSRC = $(wildcard $(SDIR)/*.c)
COBJS = ${CSRC:.c=.o}

TESTS = $(wildcard $(TDIR)/*.c)
TOBJ = $(TESTS:.c=.o)

TEST_TARGET = runtests

.PHONY: all makedebug
all: $(TARGET)

makedebug:
	@echo make degub is on
	@echo $(COBJS)

$(TARGET): $(COBJS)
	$(CC) $(CFLAGS) $(COBJS) -o $(TARGET)

$(SDIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# TESTS
.PHONY: tests
tests: $(TEST_TARGET)

$(TEST_TARGET): $(COBJS) $(TOBJ)
	$(CC) $(CFLAGS) $(COBJS) $(TOBJ) -o $(TEST_TARGET) 

$(TDIR)/%.o: $(TDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# CLEAN
.PHONY: clean

clean:
	rm -f $(COBJS) $(TOBJ) $(TARGET) $(TEST_TARGET)
