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

bool __is_lattice_ulp(int lu){ return ((lu % 2) == 1);}

unsigned long long invert(unsigned long long value){
  return (PENV->tables[__INV_TABLE])[(value >> 1) - 1];
}

void exact_arithmetic_division(PBound *dest, PTile lhs, PTile rhs)
{
  bool res_sign = is_pf_negative(lhs) ^ is_pf_negative(rhs);
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);
  unsigned long long res_lattice = 0;

  long long res_epoch = pf_epoch(lhs) - pf_epoch(rhs) - ((lhs_lattice < rhs_lattice) ? 1 : 0);

  bool res_inverted = res_epoch < 0;

  int search_index = muldiv_index(lhs_lattice, rhs_lattice);

  if (res_inverted) {
    res_epoch = (-res_epoch) - 1;

    if (rhs_lattice == 0){
      res_lattice = (PENV->tables[__INV_TABLE])[(lhs_lattice >> 1) - 1];
    } else if (lhs_lattice == 0) {
      res_lattice = rhs_lattice;
    } else {
      res_lattice = (PENV->tables[__DIV_INV_TABLE])[search_index];
    }

    if (res_lattice == 0) {res_epoch += 1;}

  } else {
    if (rhs_lattice == 0){
      res_lattice = lhs_lattice;
    } else if (lhs_lattice == 0) {
      res_lattice = (PENV->tables[__INV_TABLE])[(rhs_lattice >> 1) - 1];
    } else {
      res_lattice = (PENV->tables[__DIV_TABLE])[search_index];
    }
  }

  set_single(dest, pf_synth(res_sign, res_inverted ^ is_pf_inverted(lhs), res_epoch, res_lattice));
  return;
};
