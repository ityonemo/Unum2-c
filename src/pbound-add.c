#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

const verbose = false;

int addsub_index(long long lhs_lattice, long long rhs_lattice){
  //matrix points are generated by shifting away the ulp status.
  int lpoint1 = (lhs_lattice >> 1);
  int lpoint2 = (rhs_lattice >> 1);
  //shift the left value index by the number of lattice bits.
  return (lpoint1 << (PENV->latticebits - 1)) + lpoint2;
}

//addsub index when we have to consider moving between tables.
int table_addsub_index(int table, long long lhs_lattice, long long rhs_lattice){
  int lpoint1 = (lhs_lattice >> 1);
  int lpoint2 = (rhs_lattice >> 1);
  int tablewidth = (PENV->latticebits - 1);
  return table * tablewidth * tablewidth + (lpoint1 << (tablewidth)) + lpoint2;
}

static void exact_arithmetic_addition_crossed(PBound *dest, PTile lhs, PTile rhs){

  int res_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);
  bool res_negative = is_pf_negative(lhs);
  int table = pf_epoch(rhs) + res_epoch;

  if (table < PENV->table_counts[__ADD_CROSSED_TABLE]){
    res_lattice = (PENV->tables[__ADD_CROSSED_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
    res_epoch += (res_lattice < lhs_lattice) ? 1 : 0;

    set_single(dest, pf_synth(res_negative, false, res_epoch, res_lattice));
  } else {
    set_single(dest, res_negative ? last(lhs) : next(lhs));
  }
}

static void exact_arithmetic_addition_inverted(PBound *dest, PTile lhs, PTile rhs){

  int res_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);
  bool res_negative = is_pf_negative(lhs);
  bool res_inverted = is_pf_inverted(lhs);
  int table = pf_epoch(rhs) - res_epoch;

  if (table < PENV->table_counts[__ADD_INVERTED_TABLE]) {
    res_lattice = (PENV->tables[__ADD_INVERTED_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
    res_epoch -= (res_lattice > lhs_lattice) ? 1 : 0;
  } else {
    set_single(dest, res_negative ? last(lhs) : next(lhs));
    return;
  }

  if (res_epoch < 0){
    res_inverted = false;
    res_epoch = 0;
    res_lattice = (PENV->tables[__INVERTED_ADD_INVERTED_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
  }

  set_single(dest, pf_synth(res_negative, res_inverted, res_epoch, res_lattice));
}

static void exact_arithmetic_addition_uninverted(PBound *dest, PTile lhs, PTile rhs){
  int res_epoch = pf_epoch(lhs);
  unsigned long long res_lattice;
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);
  bool res_negative = is_pf_negative(lhs);
  int table = res_epoch - pf_epoch(rhs);

  if (table < PENV->table_counts[__ADD_TABLE]){
    res_lattice = (PENV->tables[__ADD_TABLE])[table_addsub_index(table, lhs_lattice, rhs_lattice)];
    res_epoch += (res_lattice < lhs_lattice) ? 1 : 0;

    set_single(dest, pf_synth(res_negative, false, res_epoch, res_lattice));
  } else {
    set_single(dest, res_negative ? last(lhs) : next(lhs));
  }
}

static void exact_arithmetic_addition(PBound *dest, PTile lhs, PTile rhs){
  //swap the order of the two terms to make sure that the outer float appears
  //first.
  bool orderswap = is_pf_negative(lhs) ^ (__s(lhs) < __s(rhs));

  PTile outer = orderswap ? rhs : lhs;
  PTile inner = orderswap ? lhs : rhs;

  if (is_pf_inverted(outer) ^ is_pf_inverted(inner)) {
    exact_arithmetic_addition_crossed(dest, outer, inner);
  } else if (is_pf_inverted(outer)) {
    exact_arithmetic_addition_inverted(dest, outer, inner);
  } else {
    exact_arithmetic_addition_uninverted(dest, outer, inner);
  }
}

static void pf_exact_add(PBound *dest, PTile lhs, PTile rhs){

  if (verbose){
    printf("----\n");
    printf("pf_exact_add called as, %016llx + %016llx\n", lhs, rhs);
  }

  //redo the checks on this in case we've been passed from inexact_add.
  if (is_pf_inf(lhs)) {set_inf(dest); return;}
  if (is_pf_inf(rhs)) {set_inf(dest); return;}
  if (is_pf_zero(lhs)) {set_single(dest, rhs); return;}
  if (is_pf_zero(rhs)) {set_single(dest, lhs); return;}

  if (is_pf_negative(lhs) ^ is_pf_negative(rhs)){
    exact_arithmetic_subtraction(dest, lhs, pf_additiveinverse(rhs));

    if (verbose){
      printf("pf_exact_add resulted in DEST: (via exact arithmetic subtraction) \n");
      printf("%016llX -> %016llX (%X) \n", dest->lower, dest->upper, dest->state);
    }

  } else {
    exact_arithmetic_addition(dest, lhs, rhs);
    if (verbose){
      printf("pf_exact_add resulted in DEST: (via exact arithmetic addition)\n");
      printf("%016llX -> %016llX (%X) \n", dest->lower, dest->upper, dest->state);
    }
  }
}

static void pf_inexact_add(PBound *dest, PTile lhs, PTile rhs){
  PBound temp = __EMPTYBOUND;
  pf_exact_add(&temp, glb(lhs), glb(rhs));
  PTile _l = upper_ulp(temp.lower);
  pf_exact_add(&temp, lub(lhs), lub(rhs));
  PTile _u = (temp.state == SINGLETON) ? lower_ulp(temp.lower) : lower_ulp(temp.upper);
  set_bound(dest, _l, _u);
  collapseifsingle(dest);
}

static void pf_add(PBound *dest, PTile lhs, PTile rhs){
  if (is_pf_inf(lhs)) {set_inf(dest); return;}
  if (is_pf_inf(rhs)) {set_inf(dest); return;}
  if (is_pf_zero(lhs)) {set_single(dest, rhs); return;}
  if (is_pf_zero(rhs)) {set_single(dest, lhs); return;}

  if (is_pf_exact(lhs) && is_pf_exact(rhs)) {
    pf_exact_add(dest, lhs, rhs);
  } else {
    if (verbose) {printf("pf_add sent to pf_inexact_add\n");}
    pf_inexact_add(dest, lhs, rhs);
  }
}

void add(PBound *dest, const PBound *lhs, const PBound *rhs){

  if (verbose) {
    //check the inbound data
    printf("hey, entering add with \n");
    printf("LHS:");
    printf("%016llX -> %016llX (%X) \n", lhs->lower, lhs->upper, lhs->state);
    printf("RHS:");
    printf("%016llX -> %016llX (%X) \n", rhs->lower, rhs->upper, rhs->state);
  };

  /*first handle all the corner cases*/
  if (isempty(lhs) || isempty(rhs)) {set_empty(dest); return;}
  if (isallpreals(lhs) || isallpreals(rhs)) {set_allreals(dest); return;}

  if (issingle(lhs) && issingle(rhs)) {pf_add(dest, lhs->lower, rhs->lower);

    if (verbose) {
      printf("hey, exiting add with \n");
      printf("DEST:");
      printf("%016llX -> %016llX (%X) \n", dest->lower, dest->upper, dest->state);
    };

    return;}

  //assign proxy values because "single" doesn't have a meaningful upper value.
  PTile lhs_upper_proxy = issingle(lhs) ? lhs->lower : lhs->upper;
  PTile rhs_upper_proxy = issingle(rhs) ? rhs->lower : rhs->upper;

  //NB: consider going directly to "single_add" with a double single check.
  //go ahead and do both sides of the bound calculation.  Use a temporary variable.
  PBound temp = __EMPTYBOUND;
  dest->state = STDBOUND;
  pf_add(&temp, lhs->lower, rhs->lower);
  dest->lower = temp.lower;
  pf_add(&temp, lhs_upper_proxy, rhs_upper_proxy);
  dest->upper = issingle(&temp) ? temp.lower : temp.upper;

  if (roundsinf(lhs) || roundsinf(rhs)){
    // let's make sure our result still rounds inf, this is a property which is
    // invariant under addition.  Losing this property suggests that the answer
    // should be recast as "allreals."  While we're at it, check to see if the
    // answer ends now "touch", which makes them "allreals".
    if (__s(dest->lower) <= __s(dest->upper)) {
      set_allreals(dest);
    } else if (next(dest->upper) == dest->lower) {
      set_allreals(dest);
    }
  }

  collapseifsingle(dest);
}
