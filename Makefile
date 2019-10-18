VPATH=src/:obj/:src/tests/

ODIR = obj
SDIR  = src
TARG = chip8
HDIR = ./include

CFLAGS = -g -Wall -I$(HDIR)

_OBJS = phases.o dbgutils.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

HDRS = phases.h dbgutils.h chip8.h

_TEST_OBJS = testall.o test-phases.o
TEST_OBJS = $(patsubst %, $(ODIR)/%, $(_TEST_OBJS))

TEST_HDRS = tests.h

TEST_TARG = runtests

.PHONY: all
all: $(TARG)

$(TARG): $(OBJS) $(ODIR)/chip8.o  
	gcc $(CFLAGS) $(OBJS) $(ODIR)/chip8.o -o $(TARG)

$(ODIR)/%.o: $(SDIR)/%.c $(HDRS)
	gcc $(CFLAGS) -c $< -o $@

# ===== TESTS =====
$(TEST_TARG): $(OBJS) $(TEST_OBJS)
	gcc $(CFLAGS) $(OBJS) $(TEST_OBJS) -o $(TEST_TARG)

$(ODIR)/%.o: $(SDIR)/tests/%.c $(TEST_HDRS) $(HDRS)
	gcc $(CFLAGS) -c $< -o $@
	

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(TARG) $(TEST_TARG)