#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

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
//  printf("entering exact arithmetic division \n");
  long long res_epoch = pf_epoch(lhs) - pf_epoch(rhs);
  bool res_inverted = is_pf_inverted(lhs);

  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);
  unsigned long long res_lattice = 0;
  unsigned long long res_lattice_inv = 0;

  bool invertvalues = false;

  //the lattice tables don't do zero values.
  if (lhs_lattice == 0){
    invertvalues = true;
    res_epoch -= 1;
  } else if (rhs_lattice == 0){
    res_lattice = lhs_lattice;
  } else { //do a lookup.
//    printf("we're here:  lhs: %llX, rhs: %llX, index: %i\n", lhs_lattice, rhs_lattice, muldiv_index(lhs_lattice, rhs_lattice));
    res_lattice = (PENV->tables[__DIV_TABLE])[muldiv_index(lhs_lattice, rhs_lattice)];
    res_lattice_inv = (PENV->tables[__DIV_INV_TABLE])[muldiv_index(lhs_lattice, rhs_lattice)];
    res_epoch -= res_lattice > lhs_lattice ? 1 : 0;
    //check to see if we need to go to a lower epoch.
  }

  bool res_sign = is_pf_negative(lhs) ^ is_pf_negative(rhs);

  if (res_epoch < 0){
//    printf("yo, gonna invert\n");
    res_inverted = !is_pf_inverted(lhs);
    res_epoch = (-res_epoch) - 1;
    invertvalues = !invertvalues;
  }

//  printf("norm_value %llX\n",res_lattice);
//  printf("inv_value %llX\n" ,res_lattice_inv);


  set_single(dest, pf_synth(res_sign, res_inverted, res_epoch, invertvalues ? res_lattice_inv : res_lattice));
  return;
};
