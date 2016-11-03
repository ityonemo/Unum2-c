#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/PTile.h"

void sub(PBound *dest, const PBound *lhs, const PBound *rhs){
  //allocate on the stack the rhs value, invert it, then add.
  PBound r_inv = __EMPTYBOUND;
  pcopy(&r_inv, rhs);
  additiveinverse(&r_inv);
  sub(dest, lhs, &r_inv);
}

static void exact_arithmetic_subtraction_uninverted(PBound * dest, PTile lhs, PTile rhs){
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
    PTile _l = upper_ulp(pf_synth(res_sign, true, res_epoch, invert(res_lattice + 1)));
    PTile _u = lower_ulp(pf_synth(res_sign, true, res_epoch, invert(res_lattice - 1)));
    set_bound(dest, _l, _u);
    collapseifsingle(dest);
  } else {
    set_single(dest, pf_synth(res_sign, true, res_epoch, invert(res_lattice)));
  }
}

static void exact_arithmetic_subtraction_inverted(PBound * dest, PTile lhs, PTile rhs){
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

static void exact_arithmetic_subtraction_crossed(PBound *dest, PTile lhs, PTile rhs){
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
    PTile _l = upper_ulp(pf_synth(res_sign, true, res_epoch, invert(res_lattice + 1)));
    PTile _u = lower_ulp(pf_synth(res_sign, true, res_epoch, invert(res_lattice - 1)));
    set_bound(dest, _l, _u);
    collapseifsingle(dest);
  } else {
    set_single(dest, pf_synth(res_sign, true, res_epoch, invert(res_lattice)));
  }
}

void exact_arithmetic_subtraction(PBound *dest, PTile lhs, PTile rhs){
  //check to see if they're equal.
  if (lhs == rhs) {set_zero(dest); return;}

  //the numbers should have the same sign.  But we should order them so that the
  //first one is outer, and the second one is inner.

  bool signswap = is_pf_negative(lhs) ^ (__s(lhs) < __s(rhs));

  PTile outer = signswap ? rhs : lhs;
  PTile inner = signswap ? lhs : rhs;

  if (is_pf_inverted(outer) ^ is_pf_inverted(inner)) {
    exact_arithmetic_subtraction_crossed(dest, outer, inner);
  } else if (is_pf_inverted(outer)) {
    exact_arithmetic_subtraction_inverted(dest, outer, inner);
  } else {
    exact_arithmetic_subtraction_uninverted(dest, outer, inner);
  }

  if (signswap) {additiveinverse(dest);}
}
