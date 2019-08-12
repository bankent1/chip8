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

# chip8: chip8.o phases.o
# 	gcc -g -Wall chip8.o phases.o -o target/chip8

# chip8.o: chip8.c phases.h
# 	gcc -g -c -Wall chip8.c -o target/chip8.o

# phases.o: phases.c phases.h
# 	gcc -g -c -Wall phases.c -o target/phases.o

# .PHONY: clean

# clean:
# 	rm -f *.o chip8