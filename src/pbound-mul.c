#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"
#include <stdio.h>

void single_mul(PBound *dest, const PBound *lhs, const PBound *rhs);
void inf_mul(PBound *dest, const PBound *lhs, const PBound *rhs);
void zero_mul(PBound *dest, const PBound *lhs, const PBound *rhs);

void exact_arithmetic_multiplication(PBound *dest, PFloat lhs, PFloat rhs);
void mul_pf_single(PBound *dest, PFloat lhs, PFloat rhs);
void mul_lower(PBound *dest, PFloat lhs, PFloat rhs);
void mul_upper(PBound *dest, PFloat lhs, PFloat rhs);

extern void exact_arithmetic_division(PBound *dest, PFloat lhs, PFloat rhs);
void pf_exact_mul(PBound *dest, PFloat lhs, PFloat rhs);
void pf_inexact_mul(PBound *dest, PFloat lhs, PFloat rhs);

void mul(PBound *dest, const PBound *lhs, const PBound *rhs){

  TRACK("entering mul...")
  /*first handle all the corner cases*/

  if (isempty(lhs) || isempty(rhs)) {set_empty(dest); return;}
  if (isallpreals(lhs) || isallpreals(rhs)) {set_allreals(dest); return;}

  if (issingle(lhs)) {single_mul(dest, lhs, rhs); return;}
  if (issingle(rhs)) {single_mul(dest, rhs, lhs); return;}

  if (roundsinf(lhs)) {inf_mul(dest, lhs, rhs); return;}
  if (roundsinf(rhs)) {inf_mul(dest, rhs, lhs); return;}

  if (roundszero(lhs)) {zero_mul(dest, lhs, rhs); return;}
  if (roundszero(rhs)) {zero_mul(dest, rhs, lhs); return;}

  /*handle the base case of two well-ordered bounds*/
  bool flip_sign = is_pf_negative(lhs->lower) ^ is_pf_negative(rhs->lower);

  //calculate the lower side.
  mul_lower(dest, lhs->lower, rhs->lower);
  mul_upper(dest, lhs->upper, rhs->upper);

  if (flip_sign) {additiveinverse(dest);};

  return;
}

void single_mul(PBound *dest, const PBound *lhs, const PBound *rhs)
{
  TRACK("entering single_mul...")

  if (lhs->lower == __one) {pcopy(dest, rhs); return;}
  if (lhs->lower == (__one | __inf)) {pcopy(dest, rhs); additiveinverse(dest); return;}

  if (issingle(rhs)){
    mul_pf_single(dest, lhs->lower, rhs->lower);
  } else {

    if (is_pf_inf(lhs->lower)){
      if (roundszero(rhs)){
        set_allreals(dest);
      } else {
        set_single(dest, __inf);
      }
      return;
    }

    if (is_pf_zero(lhs->lower)){
      if (roundsinf(rhs)){
        set_allreals(dest);
      } else {
        set_single(dest, __zero);
      }
      return;
    }
    //need to figure out some parity things here.
    //mul_pf_single(dest, lhs->lower, rhs->lower, LOWER);
    //mul_pf_single(dest, rhs->lower, rhs->upper, UPPER);
  }
  collapseifsingle(dest);
};

void mul_pf_single(PBound *dest, PFloat lhs, PFloat rhs)
{
  //////////////////////////////////////////////////////////////////////////////
  // check all zero and inf cases.

  if (is_pf_inf(rhs)){
    if (is_pf_zero(lhs)) {
      set_allreals(dest);
    } else {
      set_inf(dest);
    }
    return;
  }

  if (is_pf_zero(rhs)){
    if (is_pf_inf(lhs)) {
      set_allreals(dest);
    } else {
      set_zero(dest);
    }
    return;
  }

  if (is_pf_inf(lhs)) {set_inf(dest); return;}
  if (is_pf_zero(lhs)) {set_zero(dest); return;}

  //////////////////////////////////////////////////////////////////////////////

  if (is_pf_exact(lhs) && is_pf_exact(rhs)){
    pf_exact_mul(dest, lhs, rhs);
  }else{
    pf_inexact_mul(dest, lhs, rhs);
  }
}

void pf_exact_mul(PBound *dest, PFloat lhs, PFloat rhs){
  TRACK("entering pf_exact_mul...")
  //this needs to be here to avoid parity tests causing strange results.
  if (is_pf_inf(lhs) || is_pf_inf(rhs)) {set_inf(dest); return;}
  if (is_pf_zero(lhs) || is_pf_zero(rhs)) {set_zero(dest); return;}
  if (lhs == __one) {set_single(dest, rhs); return;}
  if (rhs == __one) {set_single(dest, lhs); return;}
  if (lhs == (__one | __inf)) {set_single(dest, pf_additiveinverse(rhs)); return;}
  if (rhs == (__one | __inf)) {set_single(dest, pf_additiveinverse(lhs)); return;}

  if (is_pf_inverted(lhs) ^ is_pf_inverted(rhs)){
    exact_arithmetic_division(dest, lhs, pf_multiplicativeinverse(rhs));
  } else {
    exact_arithmetic_multiplication(dest, lhs, rhs);
  }
}

bool __resultparity(PFloat lhs, PFloat rhs){
  return is_pf_negative(lhs) ^ is_pf_negative(rhs);
}

///////////////////////////////////////////////////////////////////////////////
// INNER, OUTER, UPPER, and LOWER inexact mults.

PFloat pf_inexact_mul_outer(PFloat lhs, PFloat rhs){

  TRACK("entering pf_inexact_mul_outer...")
  //calculate proper outer bounds.
  PFloat _outer_lhs = is_pf_positive(lhs) ? lub(lhs) : glb(lhs);
  PFloat _outer_rhs = is_pf_positive(rhs) ? lub(rhs) : glb(rhs);

  //create a temporary PBound to hold the prospective value.
  PBound res_temp = __EMPTYBOUND;
  pf_exact_mul(&res_temp, _outer_lhs, _outer_rhs);
  if (issingle(&res_temp)) {return res_temp.lower;};
  return is_pf_positive(res_temp.lower) ? res_temp.upper : res_temp.lower;
}

PFloat pf_inexact_mul_inner(PFloat lhs, PFloat rhs){
  TRACK("entering pf_inexact_mul_inner...")

  PFloat _inner_lhs = is_pf_positive(lhs) ? glb(lhs) : lub(lhs);
  PFloat _inner_rhs = is_pf_positive(rhs) ? glb(rhs) : lub(rhs);

  //create a temporary PBound to hold the prospective value.
  PBound res_temp = __EMPTYBOUND;
  pf_exact_mul(&res_temp, _inner_lhs, _inner_rhs);
  if (issingle(&res_temp)) {return res_temp.lower;};
  return is_pf_negative(res_temp.lower) ? res_temp.upper : res_temp.lower;
}

PFloat pf_inexact_mul_lower(PFloat lhs, PFloat rhs){
  TRACK("entering pf_inexact_mul_lower...")

  return upper_ulp(__resultparity(lhs, rhs) ? pf_inexact_mul_outer(lhs, rhs) : pf_inexact_mul_inner(lhs, rhs));
}

PFloat pf_inexact_mul_upper(PFloat lhs, PFloat rhs){

  TRACK("entering pf_inexact_mul_upper....")

  return lower_ulp(__resultparity(lhs, rhs) ? pf_inexact_mul_inner(lhs, rhs) : pf_inexact_mul_outer(lhs, rhs));
}

////////////////////////////////////////////////////////////////////////////////
// FULL inexact mult.

void pf_inexact_mul(PBound *dest, PFloat lhs, PFloat rhs){

  TRACK("entering pf_inexact_mul...")

  PFloat _l = pf_inexact_mul_lower(lhs, rhs);
  PFloat _u = pf_inexact_mul_upper(lhs, rhs);
  set_bound(dest, _l, _u);
}

int muldiv_index(long long lattice1, long long lattice2){
  //matrix points are generated by shifting away the ulp status and subtracting
  //one, because c arrays are zero-indexed.
  int lpoint1 = ((lattice1 >> 1) - 1);
  int lpoint2 = ((lattice2 >> 1) - 1);
  //shift the left value index by the number of lattice bits.
  return (lpoint1 << PENV->latticebits) + lpoint2;
}

//for now, let's make this output single.
void exact_arithmetic_multiplication(PBound *dest, PFloat lhs, PFloat rhs){

  long long res_epoch = pf_epoch(lhs) + pf_epoch(rhs);
  unsigned long long lhs_lattice = pf_lattice(lhs);
  unsigned long long rhs_lattice = pf_lattice(rhs);
  unsigned long long res_lattice = 0;

  //lattice values of zero aren't stored in the lookup matrix.
  if (lhs_lattice == 0) {
    res_lattice = rhs_lattice;
  } else if (rhs_lattice == 0) {
    res_lattice = lhs_lattice;
  } else {
    res_lattice = (PENV->tables[__MUL_TABLE])[muldiv_index(lhs_lattice, rhs_lattice)];
    //check to see if we need to modify the epoch
    if (res_lattice < lhs_lattice) {res_epoch += 1;}
  }

  //check for sign issues.
  bool res_sign = is_pf_negative(lhs) ^ is_pf_negative(rhs);
  set_single(dest, pf_synth(res_sign, is_pf_inverted(lhs), res_epoch, res_lattice));
  return;
}

void inf_mul(PBound *dest, const PBound *lhs, const PBound *rhs){};
void zero_mul(PBound *dest, const PBound *lhs, const PBound *rhs){};

void mul_single(PBound *dest, PFloat lhs, PFloat rhs){};

void mul_lower(PBound *dest, PFloat lhs, PFloat rhs){};
void mul_upper(PBound *dest, PFloat lhs, PFloat rhs){};
