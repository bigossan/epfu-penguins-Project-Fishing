IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = board.h display.h interactive.h movement.h placement.h map_generator.h auto_mode.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ_Main = main.o board.o interactive.o display.o movement.o placement.o map_generator.o auto_mode.o
OBJ_Main = $(patsubst %,$(ODIR)/%,$(_OBJ_Main))

_OBJ_board_demo = board_demo.o board.o display.o map_generator.o
OBJ_board_demo = $(patsubst %,$(ODIR)/%,$(_OBJ_board_demo))

_OBJ_map_generator2_demo = map_generator2_demo.o map_generator.o board.o display.o
OBJ_map_generator2_demo = $(patsubst %,$(ODIR)/%,$(_OBJ_map_generator2_demo))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ_Main)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
board_demo: $(OBJ_board_demo)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
map_generator2_demo: $(OBJ_map_generator2_demo)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 