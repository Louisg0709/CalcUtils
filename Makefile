IDIR = include
CC = clang
CFLAGS = -I$(IDIR)

ODIR = obj
LDIR = lib
SDIR = src
TDIR = tests

LIBS = -lm

_DEPS = SymbolicAlgebra.h MyDataStructures.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Core source object files
_OBJ = SymbolicAlgebra.o MyDataStructures.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Test object files
TEST_OBJ_SA = $(ODIR)/AlgebraTest.o
TEST_OBJ_DS = $(ODIR)/DataStructureTests.o

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(TDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

AlgebraTest: $(OBJ) $(TEST_OBJ_SA)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

DataStructureTests: $(OBJ) $(TEST_OBJ_DS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

all: AlgebraTest DataStructureTests

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ AlgebraTest DataStructureTests
