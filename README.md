# Unum2-c

A Type-2 Unum (pfloat; "projective float") computational library.

How to use:

1.  Download the package or git clone from github.
2.  make lib creates a shared library in libpfloat.so in the root directory.

You'll want to define a Unum lattice schema, using the function
```
PEnv *create_ptile_environment(double *lattice, int latticebits, double stride)
```

This function is passed an array of lattice points.  These are the positive, > 1
components of the lattice; there should be (2^latticebits - 1) values in this array.
For example, the 4-bit Type-2 Unum with exact points {Inf, -2, -1, -1/2, 0, 1/2, 1, 2}
should have a lattice [2], since that is the only component between 1 and infinity.

Given the lattice, latticebits, epochbits, and stride, the function will
automatically fill out the lookup tables for the desired lattices, and return a
pointer to the environment variable.  There is a global variable pointer PENV
which should be assigned to this location, which when enabled will allow all of
the mathematics occur in the desired Type 2 Unum environment.  You may switch
environments later, but it is the responsibility of the programmer to keep
track of which environment the Unum calculations should be run in; C will do
*nothing* to stop you from using a Unum generated in one environment in another
environment.

PTiles
=======

PTiles in this library are 64-bit integers.  They correspond to the binary forms
described in the Gustafson spec, *left aligned*.  for example, the 4-bit unum
value 2 (0b0110) is represented in this library as 0x6000000000000000.  This
enables the library to take advantage of the innate cyclical nature of ℤ/(2^64)ℤ
which is the signed integer type, as well as hardware signed integer comparison
for ordering primitives.  Moreover, key constants, such as infinity, zero,
one, and negative one, as well as masks for determining positivity, etc. are
invariant across all PTile types regardless of bitlength, simplifying status
testing pervasive in interval math operations.

NB:  The size of the PTile may be changed in a future revision, e.g. to 32 bit
integer.

PBounds
=======

PBounds in this library are the primary data type.  You can initialize PBounds
using the following functions:

```
void set_empty(PBound *dest);
void set_single(PBound *dest, PTile f);
void set_bound(PBound *dest, PTile lower, PTile upper);
void set_allreals(PBound *dest);
```

which initialize the PBound to emptyset, a single PTile, an interval of contiguous
PTiles, or all projective real numbers, respectively.

the following mathematical operations are available (NB: these may be subject to
  name changes to avoid namespace collisions):

```
void add(PBound *dest, const PBound *lhs, const PBound *rhs);
void sub(PBound *dest, const PBound *lhs, const PBound *rhs);
void mul(PBound *dest, const PBound *lhs, const PBound *rhs);
void div(PBound *dest, const PBound *lhs, const PBound *rhs);
```

If you need a floating point lattice with exact values that are not easily
representable using IEEE floating points, I recommend using the Julia library:

https://github.com/ityonemo/unum2

This production of this work was supported by the [A*STAR institute](https://www.a-star.edu.sg/)
[Computational Resource Centre](https://www.acrc.a-star.edu.sg/) (A*CRC).
