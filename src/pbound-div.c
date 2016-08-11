#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"
#include <stdio.h>

int muldiv_index(long long lhs_lattice, long long rhs_lattice);

bool __is_lattice_ulp(int lu){ return ((lu % 2) == 1);}
unsigned long long invert(unsigned long long value){
  return (PENV->tables[__INV_TABLE])[value >> 1];
}

void exact_arithmetic_division(PBound *dest, PFloat lhs, PFloat rhs)
{
  long long res_epoch = pf_epoch(lhs) - pf_epoch(rhs);
  bool res_inverted = is_pf_inverted(lhs);

  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);
  unsigned long long res_lattice = 0;

  bool invertvalues = false;

  //the lattice tables don't do zero values.
  if (lhs_lattice == 0){
    invertvalues = true;
    res_epoch -= 1;
  } else if (rhs_lattice == 0){
    res_lattice = lhs_lattice;
  } else { //do a lookup.
    res_lattice = (PENV->tables[__DIV_TABLE])[muldiv_index(lhs_lattice, rhs_lattice)];
    res_epoch -= res_lattice > lhs_lattice ? 1 : 0;
    //check to see if we need to go to a lower epoch.
  }

  bool res_sign = is_pf_negative(lhs) ^ is_pf_negative(rhs);

  if (res_epoch < 0){
    res_inverted = !is_pf_inverted(lhs);
    res_epoch = (-res_epoch) - 1;
    invertvalues = !invertvalues;
  }

  if (!invertvalues) {
    set_single(dest, pf_synth(res_sign, res_inverted, res_epoch, res_lattice));
  } else if (__is_lattice_ulp(res_lattice)) {
    PFloat _l = upper_ulp(pf_synth(res_sign, res_inverted, res_epoch, invert(res_lattice - 1)));
    PFloat _u = lower_ulp(pf_synth(res_sign, res_inverted, res_epoch, invert(res_lattice + 1)));
    set_bound(dest, _l, _u);
    collapseifsingle(dest);
  } else {
    set_single(dest, pf_synth(res_sign, res_inverted, res_epoch, invert(res_lattice)));
  }
  return;
};
