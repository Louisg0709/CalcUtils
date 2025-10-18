IDIR = include
CC=clang
CFLAGS=-I$(IDIR)

ODIR= obj
LDIR = lib
SDIR = src

LIBS=-lm

_DEPS = SymbolicAlgebra.h MyDataStructures.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = SymbolicAlgebra.o MyDataStructures.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

SymbolicAlgebra: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ SymbolicAlgebra