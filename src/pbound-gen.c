#include "../include/pbound.h"
#include "../include/pfloat.h"

// GENERATIVE PBound functions

//specific singleton values.
void set_zero(PBound *dest){
  dest->state = SINGLETON;
  dest->lower = __zero;
};

void set_inf(PBound *dest){
  dest->state = SINGLETON;
  dest->lower = __inf;
};

void set_one(PBound *dest){
  dest->state = SINGLETON;
  dest->lower = __one;
};

void set_neg_one(PBound *dest){
  dest->state = SINGLETON;
  dest->lower = __one | __inf;
};

//NB: STILL NEEDS IMPLEMENTATION.
void set_f32(PBound *dest, float f);
void set_f64(PBound *dest, double f);
void set_int(PBound *dest, long long i);

//initialization as special values.
void set_empty(PBound *dest){
  dest->state = EMPTYSET;
};

void set_single(PBound *dest, PFloat value){
  dest->state = SINGLETON;
  dest->lower = value;
};
void set_bound(PBound *dest, PFloat lower, PFloat upper){
  dest->state = STDBOUND;
  dest->lower = lower;
  dest->upper = upper;
};
void set_allreals(PBound *dest){
  dest->state = ALLREALS;
};

void pcopy(PBound *dest, const PBound *src){
  dest->state = src->state;
  dest->lower = src->lower;
  dest->upper = src->upper;
}
