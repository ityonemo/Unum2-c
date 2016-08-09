#library headers
IDIR = include
#internal headers
HDIR = src
#source files
SDIR = src
#object stash
ODIR = obj
#test directory
TDIR = test
#flags for making executable
CFLAGS = -I.

#header files & dependency chain
_IDEPS = pbound.h penv.h pfloat.h
IDEPS = $(patsubst %,$(IDIR)/%,$(_IDEPS))
_HDEPS = PFloat4.h
HDEPS = $(patsubst %,$(HDIR)/%,$(_HDEPS))
DEPS = $(IDEPS) $(HDEPS)
_TDEPS = PFloat4-test.h
TDEPS = $(patsubst %,$(TDIR)/%,$(_TDEPS))
#general objects
_OBJ = PFloat4.o pbound-cmp.o pbound-desc.o pbound-gen.o pbound-prop.o penv.o pfloat-iter.o pfloat-prop.o pfloat-math-inv.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TEST = pfloat-test.o PFloat4-test.o PFloat4-paritytests.o PFloat4-inversetests.o
TEST = $(patsubst %,$(TDIR)/%,$(_TEST))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $<

$(TDIR)/%.o: $(TDIR)/%.c $(DEPS) $(TDEPS)
	$(CC) -c -o $@ $<

test: clean $(OBJ) $(TEST) $(DEPS) $(TDEPS)
	$(CC) -o pfloattest $(TEST) $(OBJ)
	./pfloattest

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
