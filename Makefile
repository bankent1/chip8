VPATH=src/:obj/

ODIR = obj
SDIR  = src
TARG = chip8

CFLAGS = -g -Wall

_OBJS = chip8.o phases.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

$(TARG): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $(TARG)

$(ODIR)/%.o: $(SDIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(TARG)