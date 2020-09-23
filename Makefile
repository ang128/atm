IDIR = ./include
SDIR = ./src
EDIR = ./bin
CC=g++
CFLAGS=-I$(IDIR)

ODIR=obj
LIBS=-lrt

_DEPS = command.hpp customer.hpp machine.hpp transaction.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = command.o customer.o machine.o transaction.o atm.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	@mkdir -p obj
	@mkdir -p bin
	$(CC) -c -o $@ $< $(CFLAGS)

atm: $(OBJ)
	$(CC) -o $(EDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
