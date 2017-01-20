#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

void sub(PBound *dest, const PBound *lhs, const PBound *rhs){
  //allocate on the stack the rhs value, invert it, then add.
  PBound r_inv = __EMPTYBOUND;
  pcopy(&r_inv, rhs);
  additiveinverse(&r_inv);
  sub(dest, lhs, &r_inv);
}

static PTile exact_arithmetic_subtraction_uninverted(PTile lhs, PTile rhs){
  bool res_negative = is_pf_negative(lhs);
  bool res_inverted = false;
  long long res_epoch;
  long long rhs_epoch = pf_epoch(rhs);
  long long lhs_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);

  //figure out the table delta.

  //ascertain the table that we'll need to look up.
  int table = lhs_epoch - rhs_epoch;
  int lookup_index = table_addsub_index(table, lhs_lattice, rhs_lattice);

  if (table < PENV->table_counts[__SUB_TABLE]){
    res_lattice = (PENV->tables[__SUB_TABLE])[lookup_index];
    res_epoch = lhs_epoch - (PENV->tables[__SUB_EPOCH_TABLE])[lookup_index];
  } else {
    return (res_negative ? next(lhs) : prev(lhs));
  }

  if (res_epoch < 0){
    res_inverted = true;
    res_epoch = 0;
    res_lattice = (PENV->tables[__INVERTED_SUB_TABLE])[lookup_index];
  }

  return pf_synth(res_negative, res_inverted, res_epoch, res_lattice);
}

static PTile exact_arithmetic_subtraction_inverted(PTile lhs, PTile rhs){
  bool res_negative = is_pf_negative(lhs);
  long long res_epoch;
  long long rhs_epoch = pf_epoch(rhs);
  long long lhs_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);

  //ascertain the table that we'll need to look up.
  int table = rhs_epoch - lhs_epoch;

  if (table < PENV->table_counts[__SUB_INVERTED_TABLE]){
    res_lattice = (PENV->tables[__SUB_INVERTED_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
    res_epoch = lhs_epoch + (PENV->tables[__SUB_INVERTED_EPOCH_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
  } else {
    return (res_negative ? next(lhs) : prev(lhs));
  }

  return pf_synth(res_negative, true, res_epoch, res_lattice);
}

static PTile exact_arithmetic_subtraction_crossed(PTile lhs, PTile rhs){
  bool res_negative = is_pf_negative(lhs);
  bool res_inverted = false;
  long long res_epoch;
  long long rhs_epoch = pf_epoch(rhs);
  long long lhs_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);

  //ascertain the table that we'll need to look up.
  int table = rhs_epoch + lhs_epoch;

  if (table < PENV->table_counts[__SUB_CROSSED_TABLE]){
    res_lattice = (PENV->tables[__SUB_CROSSED_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
    res_epoch = lhs_epoch - (PENV->tables[__SUB_CROSSED_EPOCH_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
  } else {
    return (res_negative ? next(lhs) : prev(lhs));
  }

  if (res_epoch < 0){
    res_inverted = true;
    res_epoch = 0;
    res_lattice = (PENV->tables[__INVERTED_SUB_CROSSED_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
  }

  return pf_synth(res_negative, res_inverted, res_epoch, res_lattice);
}

PTile exact_arithmetic_subtraction(PTile lhs, PTile rhs){
  //check to see if they're equal.
  if (lhs == rhs) {return __zero;}

  //the numbers should have the same sign.  But we should order them so that the
  //first one is outer, and the second one is inner.

  bool signswap = is_pf_negative(lhs) ^ (__s(lhs) < __s(rhs));

  PTile outer = signswap ? rhs : lhs;
  PTile inner = signswap ? lhs : rhs;
  PTile temp;

  if (is_pf_inverted(outer) ^ is_pf_inverted(inner)) {
    temp = exact_arithmetic_subtraction_crossed(outer, inner);
  } else if (is_pf_inverted(outer)) {
    temp = exact_arithmetic_subtraction_inverted(outer, inner);
  } else {
    temp = exact_arithmetic_subtraction_uninverted(outer, inner);
  }

  return (signswap) ? pf_additiveinverse(temp) : temp;
}
