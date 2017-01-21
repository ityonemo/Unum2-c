#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

extern const verbose;

void div(PBound *dest, const PBound *lhs, const PBound *rhs){
  //allocate on the stack the rhs value, invert it, then multiply.
  PBound r_inv = __EMPTYBOUND;
  pcopy(&r_inv, rhs);
  multiplicativeinverse(&r_inv);
  mul(dest, lhs, &r_inv);
}

unsigned long long invert(unsigned long long value){
  return (PENV->tables[__INV_TABLE])[(value >> 1) - 1];
}

void dc_arithmetic_division(__dc_tile *div_result, PTile lhs, PTile rhs){
  unsigned long long lhs_lattice = tile_lattice(lhs);
  unsigned long long rhs_lattice = tile_lattice(rhs);

  div_result->epoch = tile_epoch(lhs) - tile_epoch(rhs) - ((lhs_lattice < rhs_lattice) ? 1 : 0);

  bool res_inverted = (div_result->epoch) < 0;

  int search_index = muldiv_index(lhs_lattice, rhs_lattice);

  if (res_inverted) {
    div_result->epoch = (-(div_result->epoch)) - 1;

    if (rhs_lattice == 0){
      div_result->lattice = (PENV->tables[__INV_TABLE])[(lhs_lattice >> 1) - 1];
    } else if (lhs_lattice == 0) {
      div_result->lattice = rhs_lattice;
    } else {
      div_result->lattice = (PENV->tables[__DIV_INV_TABLE])[search_index];
    }

    if (div_result->lattice == 0) {div_result->epoch += 1;}

  } else {
    if (rhs_lattice == 0){
      div_result->lattice = lhs_lattice;
    } else if (lhs_lattice == 0) {
      div_result->lattice = (PENV->tables[__INV_TABLE])[(rhs_lattice >> 1) - 1];
    } else {
      div_result->lattice = (PENV->tables[__DIV_TABLE])[search_index];
    }
  }

  div_result->inverted = is_tile_inverted(lhs) ^ res_inverted;
  div_result->negative = is_tile_negative(lhs) ^ is_tile_negative(rhs);
  return;
}
