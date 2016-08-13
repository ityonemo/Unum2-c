#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"

void exact_arithmetic_subtraction_uninverted(PBound * dest, PFloat lhs, PFloat rhs);
void exact_arithmetic_subtraction_inverted(PBound * dest, PFloat lhs, PFloat rhs);
void exact_arithmetic_subtraction_crossed(PBound * dest, PFloat lhs, PFloat rhs);

void exact_arithmetic_subtraction(PBound *dest, PFloat lhs, PFloat rhs){

  TRACK("entering exact_arithmetic_subtraction...")

  //check to see if they're equal.
  if (lhs == rhs) {set_zero(dest); return;}

  //the numbers should have the same sign.  But we should order them so that the
  //first one is outer, and the second one is inner.

  bool signswap = is_pf_negative(lhs) ^ (__s(lhs) < __s(rhs));

  PFloat outer = signswap ? rhs : lhs;
  PFloat inner = signswap ? lhs : rhs;

  if (is_pf_inverted(outer) ^ is_pf_inverted(inner)) {
    exact_arithmetic_subtraction_crossed(dest, outer, inner);
  } else if (is_pf_inverted(outer)) {
    exact_arithmetic_subtraction_inverted(dest, outer, inner);
  } else {
    exact_arithmetic_subtraction_uninverted(dest, outer, inner);
  }

  if (signswap) {additiveinverse(dest);}
}

bool __is_lattice_ulp(int lu);
unsigned long long invert(unsigned long long value);

void exact_arithmetic_subtraction_uninverted(PBound * dest, PFloat lhs, PFloat rhs){

  TRACK("entering exact_arithmetic_subtraction_uninverted...")

  bool res_sign = is_pf_negative(lhs);
  bool res_inverted = false;
  long long res_epoch;
  long long rhs_epoch = pf_epoch(rhs);
  long long lhs_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);


  if ((rhs_epoch == lhs_epoch)) {
    res_lattice = (PENV->tables[__SUB_TABLE])[addsub_index(lhs_lattice, rhs_lattice)];
    res_epoch = rhs_epoch - (PENV->tables[__SUB_EPOCH_TABLE])[addsub_index(lhs_lattice, rhs_lattice)];
  } else {
    //nothing, for now.
    return;
  }

  if (res_epoch < 0) {
    res_inverted = true;
    res_epoch = (-res_epoch) - 1;
  }

  if (!res_inverted) {
    set_single(dest, pf_synth(res_sign, false, res_epoch, res_lattice));
  } else if (__is_lattice_ulp(res_lattice)) {
    PFloat _l = upper_ulp(pf_synth(res_sign, true, res_epoch, invert(res_lattice + 1)));
    PFloat _u = lower_ulp(pf_synth(res_sign, true, res_epoch, invert(res_lattice - 1)));
    set_bound(dest, _l, _u);
    collapseifsingle(dest);
  } else {
    set_single(dest, pf_synth(res_sign, true, res_epoch, invert(res_lattice)));
  }
}

void exact_arithmetic_subtraction_inverted(PBound * dest, PFloat lhs, PFloat rhs){

  TRACK("entering exact_arithmetic_subtraction_inverted...")

  bool res_sign = is_pf_negative(lhs);
  long long res_epoch;
  long long rhs_epoch = pf_epoch(rhs);
  long long lhs_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);

  if ((rhs_epoch == lhs_epoch)) {
    res_lattice = (PENV->tables[__SUB_INVERTED_TABLE])[addsub_index(lhs_lattice, rhs_lattice)];
    res_epoch = rhs_epoch + (PENV->tables[__SUB_INVERTED_EPOCH_TABLE])[addsub_index(lhs_lattice, rhs_lattice)];
  } else {
    //nothing, for now.
    return;
  }

  set_single(dest, pf_synth(res_sign, true, res_epoch, res_lattice));
}

void exact_arithmetic_subtraction_crossed(PBound *dest, PFloat lhs, PFloat rhs){

  TRACK("entering exact_arithmetic_subtraction_crossed...")

  bool res_sign = is_pf_negative(lhs);
  bool res_inverted = false;
  long long res_epoch;
  long long rhs_epoch = pf_epoch(rhs);
  long long lhs_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);

  if ((rhs_epoch == 0) && (lhs_epoch == 0)) {
    res_lattice = (PENV->tables[__SUB_CROSSED_TABLE])[addsub_index(lhs_lattice, rhs_lattice)];
    res_epoch = rhs_epoch - (PENV->tables[__SUB_CROSSED_EPOCH_TABLE])[addsub_index(lhs_lattice, rhs_lattice)];
  } else {
    //nothing, for now.
    return;
  }

  if (res_epoch < 0) {
    res_inverted = true;
    res_epoch = (-res_epoch) - 1;
  }

  if (!res_inverted) {
    set_single(dest, pf_synth(res_sign, false, res_epoch, res_lattice));
  } else if (__is_lattice_ulp(res_lattice)) {
    PFloat _l = upper_ulp(pf_synth(res_sign, true, res_epoch, invert(res_lattice + 1)));
    PFloat _u = lower_ulp(pf_synth(res_sign, true, res_epoch, invert(res_lattice - 1)));
    set_bound(dest, _l, _u);
    collapseifsingle(dest);
  } else {
    set_single(dest, pf_synth(res_sign, true, res_epoch, invert(res_lattice)));
  }
}
