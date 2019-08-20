VPATH=src/:obj/

ODIR = obj
SDIR  = src
TARG = chip8

CFLAGS = -g -Wall

_OBJS = phases.o dbgutils.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

_TEST_OBJS = testall.o test-phases.o
TEST_OBJS = $(patsubst %, $(ODIR)/%, $(_TEST_OBJS))

.PHONY: all
all: $(TARG)

$(TARG): $(OBJS) $(ODIR)/chip8.o
	gcc $(CFLAGS) $(OBJS) $(ODIR)/chip8.o -o $(TARG)

$(ODIR)/%.o: $(SDIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

# TESTS
runtests: $(OBJS) $(TEST_OBJS)
	gcc $(CFLAGS) $(OBJS) $(TEST_OBJS) -o runtests

$(ODIR)/%.o: $(SDIR)/tests/%.c
	gcc $(CFLAGS) -c $< -o $@
	

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(TARG) runtests