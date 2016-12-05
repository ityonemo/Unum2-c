/* pmath.h ****************************************************************
 * c functions relating to binary projective floating point numbers.
 *************************************************************************/
#ifndef __PBOUND_H
#define __PBOUND_H

#include <stdbool.h>
#include "ptile.h"

typedef enum {
  EMPTYSET = 0,
  SINGLETON,
  STDBOUND,
  ALLREALS
} PState;

typedef struct {
   PTile lower;
   PTile upper;
   PState state;
} PBound;

////////////////////////////////////////////////////////////////////////////////
// PROPERTY functions
bool isempty(const PBound *v);
bool issingle(const PBound *v);
bool isbound(const PBound *v);
bool isallpreals(const PBound *v);

bool roundsinf(const PBound *v);
bool roundszero(const PBound *v);
bool isnegative(const PBound *v);
bool ispositive(const PBound *v);

void collapseifsingle(PBound *v);

////////////////////////////////////////////////////////////////////////////////
// GENERATIVE functions
void set_zero(PBound *v);
void set_inf(PBound *v);
void set_one(PBound *v);
void set_neg_one(PBound *v);

void set_f32(PBound *v, float f);
void set_f64(PBound *v, double f);
void set_int(PBound *v, long long i);

void set_empty(PBound *dest);
void set_single(PBound *dest, PTile f);
void set_bound(PBound *dest, PTile lower, PTile upper);
void set_allreals(PBound *dest);

void pcopy(PBound *dest, const PBound *src);

////////////////////////////////////////////////////////////////////////////////
// INVERSION functions
void additiveinverse(PBound *v);
void multiplicativeinverse(PBound *v);

////////////////////////////////////////////////////////////////////////////////
// COMPARISON functions
bool eq(const PBound *lhs, const PBound *rhs);  //equality
bool lt(const PBound *lhs, const PBound *rhs);  //less than
bool gt(const PBound *lhs, const PBound *rhs);  //greater than
bool in(const PBound *lhs, const PBound *rhs);  //rhs contains lhs
bool ol(const PBound *lhs, const PBound *rhs);  //lhs and rhs overlap

////////////////////////////////////////////////////////////////////////////////
// ARITHMETIC functions

void add(PBound *dest, const PBound *lhs, const PBound *rhs);
void sub(PBound *dest, const PBound *lhs, const PBound *rhs);
void mul(PBound *dest, const PBound *lhs, const PBound *rhs);
void div(PBound *dest, const PBound *lhs, const PBound *rhs);

// Arithmetic helper functions

/* pbound-add.c: */
int addsub_index(long long lhs_lattice, long long rhs_lattice);
int table_addsub_index(int table, long long lhs_lattice, long long rhs_lattice);
/* pbound-div.c: */
bool __is_lattice_ulp(int lu);
void exact_arithmetic_division(PBound *dest, PTile lhs, PTile rhs);
unsigned long long invert(unsigned long long value);
/* pbound-mul.c: */
int muldiv_index(long long lhs_lattice, long long rhs_lattice);
/* pbound-sub.c: */
void exact_arithmetic_subtraction(PBound *dest, PTile lhs, PTile rhs);


////////////////////////////////////////////////////////////////////////////////
// DESCRIPTIVE functions

//NB:  THIS INTERFACE MAY CHANGE.
void describe(PBound *value);

////////////////////////////////////////////////////////////////////////////////
// special DEFINES
#define __EMPTYBOUND {__zero, __zero, EMPTYSET}

#endif
