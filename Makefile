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
CFLAGS = -I. -fPIC
CC = cc $(CFLAGS)
#flags for making a library

#header files & dependency chain
_IDEPS = pbound.h penv.h pfloat.h
IDEPS = $(patsubst %,$(IDIR)/%,$(_IDEPS))
_HDEPS = PFloat4.h
HDEPS = $(patsubst %,$(HDIR)/%,$(_HDEPS))
DEPS = $(IDEPS) $(HDEPS)
_TDEPS = PFloat4-test.h
TDEPS = $(patsubst %,$(TDIR)/%,$(_TDEPS))
#general objects
_OBJ = pbound-cmp.o pbound-desc.o pbound-gen.o pbound-prop.o pbound-inv.o \
       pbound-mul.o pbound-div.o pbound-add.o pbound-sub.o penv.o pfloat-iter.o \
			 pfloat-prop.o pfloat-math-inv.o pfloat-synth.o penv-create.o PFloat4.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TEST = pfloat-test.o PFloat4-test.o PFloat4-paritytests.o PFloat4-inversetests.o \
        PFloat4-itertests.o PFloat4-synthtests.o PFloat4-multest.o PFloat4-addtest.o \
				PFloat4-boundmathtests.o PFloat4-creationtest.o

TEST = $(patsubst %,$(TDIR)/%,$(_TEST))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $<

$(TDIR)/%.o: $(TDIR)/%.c $(DEPS) $(TDEPS)
	$(CC) -c -o $@ $<

test: clean $(OBJ) $(TEST) $(DEPS) $(TDEPS)
	$(CC) -o pfloattest $(TEST) $(OBJ)
	./pfloattest

lib: clean $(OBJ)
	cc -shared -Wl,-soname,libpfloat.so -o libpfloat.so $(OBJ)

.PHONY: clean
clean:
	rm -rf $(ODIR)
	mkdir $(ODIR)
	rm -f $(TDIR)/*.o
