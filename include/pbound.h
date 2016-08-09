/* pmath.h ****************************************************************
 * c functions relating to binary projective floating point numbers.
 *************************************************************************/
#ifndef __PBOUND_H
#define __PBOUND_H

#include <stdbool.h>
#include "pfloat.h"

typedef enum {EMPTYSET, SINGLETON, STDBOUND, ALLREALS} PState;

typedef struct {
   PFloat lower;
   PFloat upper;
   PState state;
} PBound;

////////////////////////////////////////////////////////////////////////////////
// PROPERTY functions
bool isempty(PBound *v);
bool issingle(PBound *v);
bool isbound(PBound *v);
bool isallpreals(PBound *v);

bool roundsinf(PBound *v);
bool roundszero(PBound *v);
bool isnegative(PBound *v);
bool ispositive(PBound *v);

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
void set_single(PBound *dest, PFloat f);
void set_bound(PBound *dest, PFloat lower, PFloat upper);
void set_allreals(PBound *dest);

void pcopy(PBound *dest, PBound *src);

////////////////////////////////////////////////////////////////////////////////
// COMPARISON functions
bool eq(PBound *lhs, PBound *rhs);  //equality
bool lt(PBound *lhs, PBound *rhs);  //less than
bool gt(PBound *lhs, PBound *rhs);  //greater than
bool in(PBound *lhs, PBound *rhs);  //rhs contains lhs
bool ol(PBound *lhs, PBound *rhs);  //lhs and rhs overlap

////////////////////////////////////////////////////////////////////////////////
// ARITHMETIC functions

void add(PBound *dest, PBound *lhs, PBound *rhs);
void sub(PBound *dest, PBound *lhs, PBound *rhs);
void mul(PBound *dest, PBound *lhs, PBound *rhs);
void div(PBound *dest, PBound *lhs, PBound *rhs);

////////////////////////////////////////////////////////////////////////////////
// DESCRIPTIVE functions

//NB:  THIS INTERFACE MAY CHANGE.
void describe(PBound *value);

#endif
