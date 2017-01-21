#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

static bool __resultparity(PTile lhs, PTile rhs) {return is_tile_negative(lhs) ^ is_tile_negative(rhs);}

static void dc_arithmetic_multiplication(__dc_tile *mul_result, PTile lhs, PTile rhs){
  mul_result->epoch = tile_epoch(lhs) + tile_epoch(rhs);
  unsigned long long lhs_lattice = tile_lattice(lhs);
  unsigned long long rhs_lattice = tile_lattice(rhs);

  //lattice values of zero aren't stored in the lookup matrix.
  if (lhs_lattice == 0) {
    mul_result->lattice = rhs_lattice;
  } else if (rhs_lattice == 0) {
    mul_result->lattice = lhs_lattice;
  } else {
    mul_result->lattice = (PENV->tables[__MUL_TABLE])[muldiv_index(lhs_lattice, rhs_lattice)];

    //check to see if we need to modify the epoch
    if (mul_result->lattice < lhs_lattice) {mul_result->epoch += 1;}
  }

  mul_result->inverted = is_tile_inverted(lhs);
  mul_result->negative = is_tile_negative(lhs) ^ is_tile_negative(rhs);

  return;
}

void dc_mul(__dc_tile *mul_result, PTile lhs, PTile rhs){
  if (is_tile_inverted(lhs) ^ is_tile_inverted(rhs)){
    dc_arithmetic_division(mul_result, lhs, tile_multiplicativeinverse(rhs));
  } else {
    dc_arithmetic_multiplication(mul_result, lhs, rhs);
  }
}

static PTile tile_exact_mul(PTile lhs, PTile rhs){
  //this needs to be here to avoid parity tests causing strange results.
  if (is_tile_inf(lhs) || is_tile_inf(rhs))   {return __inf;}
  if (is_tile_zero(lhs) || is_tile_zero(rhs)) {return __zero;}
  if (lhs == __one)                           {return rhs;}
  if (rhs == __one)                           {return lhs;}
  if (lhs == (__one | __inf))                 {return tile_additiveinverse(rhs);}
  if (rhs == (__one | __inf))                 {return tile_additiveinverse(lhs);}

  __dc_tile mul_result;

  dc_mul(&mul_result, lhs, rhs);

  return tile_synth(&mul_result);
}

///////////////////////////////////////////////////////////////////////////////
// INNER, OUTER, UPPER, and LOWER inexact mults.

static PTile inexact_mul_outer_bound(PTile lhs, PTile rhs){

  //calculate proper outer bounds.
  PTile _outer_lhs = is_tile_positive(lhs) ? lub(lhs) : glb(lhs);
  PTile _outer_rhs = is_tile_positive(rhs) ? lub(rhs) : glb(rhs);

  return tile_exact_mul(_outer_lhs, _outer_rhs);
}

static PTile inexact_mul_inner_bound(PTile lhs, PTile rhs){

  PTile _inner_lhs = is_tile_positive(lhs) ? glb(lhs) : lub(lhs);
  PTile _inner_rhs = is_tile_positive(rhs) ? glb(rhs) : lub(rhs);

  return tile_exact_mul(_inner_lhs, _inner_rhs);
}

static PTile inexact_mul_lower(PTile lhs, PTile rhs){
  return upper_ulp(__resultparity(lhs, rhs) ? inexact_mul_outer_bound(lhs, rhs) : inexact_mul_inner_bound(lhs, rhs));
}

static PTile inexact_mul_upper(PTile lhs, PTile rhs){
  return lower_ulp(__resultparity(lhs, rhs) ? inexact_mul_inner_bound(lhs, rhs) : inexact_mul_outer_bound(lhs, rhs));
}

////////////////////////////////////////////////////////////////////////////////
// FULL inexact mult.

static void inexact_mul(PBound *dest, PTile lhs, PTile rhs){
  PTile _l = inexact_mul_lower(lhs, rhs);
  PTile _u = inexact_mul_upper(lhs, rhs);
  set_bound(dest, _l, _u);
}

static void mul_pf_single(PBound *dest, PTile lhs, PTile rhs)
{
  //////////////////////////////////////////////////////////////////////////////
  // check all zero and inf cases.

  if (is_tile_inf(rhs)){
    if (is_tile_zero(lhs)) {
      set_allreals(dest);
    } else {
      set_inf(dest);
    }
    return;
  }

  if (is_tile_zero(rhs)){
    if (is_tile_inf(lhs)) {
      set_allreals(dest);
    } else {
      set_zero(dest);
    }
    return;
  }

  if (is_tile_inf(lhs)) {set_inf(dest); return;}
  if (is_tile_zero(lhs)) {set_zero(dest); return;}

  //////////////////////////////////////////////////////////////////////////////

  if (is_tile_exact(lhs) && is_tile_exact(rhs)){
    set_single(dest, tile_exact_mul(lhs, rhs));
  }else{
    inexact_mul(dest, lhs, rhs);
  }
}

static void mul_lower(PBound *dest, PTile lhs, PTile rhs){
  PBound temp;
  mul_pf_single(&temp, lhs, rhs);
  if (isallpreals(&temp)){
    set_allreals(dest);
  } else {
    dest->lower = temp.lower;
  }
};

static void mul_upper(PBound *dest, PTile lhs, PTile rhs){  //bail if this problem is already solved.
  if (isallpreals(dest)) {return;}

  PBound temp;
  mul_pf_single(&temp, lhs, rhs);
  if (isallpreals(&temp)){
    set_allreals(dest);
  } else {
    dest->upper = issingle(&temp) ? temp.lower : temp.upper;
  }
};

static void single_mul(PBound *dest, const PBound *lhs, const PBound *rhs)
{
  if (lhs->lower == __one) {pcopy(dest, rhs); return;}
  if (lhs->lower == (__one | __inf)) {pcopy(dest, rhs); additiveinverse(dest); return;}

  if (issingle(rhs)){
    mul_pf_single(dest, lhs->lower, rhs->lower);
  } else {

    if (is_tile_inf(lhs->lower)){
      if (roundszero(rhs)){
        set_allreals(dest);
      } else {
        set_single(dest, __inf);
      }
      return;
    }

    if (is_tile_zero(lhs->lower)){
      if (roundsinf(rhs)){
        set_allreals(dest);
      } else {
        set_single(dest, __zero);
      }
      return;
    }

    bool lhs_neg = is_tile_negative(lhs->lower);
    PTile lhs_proxy = lhs_neg ? tile_additiveinverse(lhs->lower) : lhs->lower;

    dest->state = STDBOUND;
    mul_lower(dest, rhs->lower, lhs_proxy);
    mul_upper(dest, rhs->upper, lhs_proxy);

    if (lhs_neg) {additiveinverse(dest);};
  }
  collapseifsingle(dest);
};

int muldiv_index(long long lattice1, long long lattice2){
  //matrix points are generated by shifting away the ulp status and minus one
  //since the zero value is not part of the formula.
  int lpoint1 = (lattice1 >> 1) - 1;
  int lpoint2 = (lattice2 >> 1) - 1;
  //shift the left value index by the number of lattice bits.
  return lpoint1 * ((1 << (PENV->latticebits - 1)) - 1) + lpoint2;
}

static void inf_mul(PBound *dest, const PBound *lhs, const PBound *rhs)
{
  if (roundszero(rhs) || is_tile_zero(rhs->lower) || is_tile_zero(rhs->upper)) {
    //the case where the right hand side hits zero:
    set_allreals(dest);
    return;
  } else if (roundszero(lhs)) {
    //the case where the lhs value rounds both infinity and zero.
    //the value for rhs must be a nonflipped doubel interval on one side of zero.

    if (roundsinf(rhs)) {set_allreals(dest); return;}

    int _state = isnegative(lhs) * 1 + isnegative(rhs) * 2;

    PTile _l, _u;

    // canonical examples:
    // (100, 1) * (3, 4)     -> (300, 4)    (l * l, u * u)
    // (100, 1) * (-4, -3)   -> (-4, -300)  (u * l, l * u)
    // (-1, -100) * (3, 4)   -> (-4, -300)  (l * u, u * l)
    // (-1, -100) * (-4, -3) -> (300, 4)    (u * u, l * l)

    dest->state = STDBOUND;

    //assign upper and lower values based on the bounds
    switch (_state){
      case 0:
        mul_lower(dest, lhs->lower, rhs->lower);
        mul_upper(dest, lhs->upper, rhs->upper);
      break;
      case 1:
        mul_lower(dest, lhs->upper, rhs->lower);
        mul_upper(dest, lhs->lower, rhs->upper);
      break;
      case 2:
        mul_lower(dest, lhs->upper, rhs->lower);
        mul_upper(dest, lhs->lower, rhs->upper);
      break;
      case 3: // state == 3.
        mul_lower(dest, lhs->upper, rhs->upper);
        mul_upper(dest, lhs->lower, rhs->lower);
      break;
    }

    if (__s(dest->lower) <= __s(dest->upper)) {
      set_allreals(dest); return;
    } else if (next(dest->upper) == dest->lower) {
      set_allreals(dest); return;
    }

  } else if (roundsinf(rhs)) {
    // now we must check if y rounds infinity.
    // like the double "rounds zero" case, we have to check four possible endpoints.
    // unlinke the "rounds zero" case, the lower ones are positive valued, so that's not "crossed"

    PBound temp1 = __EMPTYBOUND;
    PBound temp2 = __EMPTYBOUND;

    mul_lower(&temp1, lhs->lower, rhs->lower);
    mul_lower(&temp2, lhs->upper, rhs->upper);
    mul_upper(&temp1, lhs->lower, rhs->upper);
    mul_upper(&temp2, lhs->upper, rhs->lower);

    dest->state = STDBOUND;

    dest->lower = (__s(temp1.lower) < __s(temp2.lower)) ? temp1.lower : temp2.lower;
    dest->upper = (__s(temp1.upper) < __s(temp2.upper)) ? temp2.upper : temp1.upper;

  } else {
    //the last case is if x rounds infinity but y is a "well-behaved" value.

    //canonical example:
    // (2, -3) * (5, 7) -> (10, -15)
    // (2, -3) * (-7, -5) -> (15, -10)

    if (isnegative(rhs)) {
      dest->state = STDBOUND;
      mul_lower(dest, lhs->upper, rhs->upper);
      mul_upper(dest, lhs->lower, rhs->upper);
    } else {
      dest->state = STDBOUND;
      mul_lower(dest, lhs->lower, rhs->lower);
      mul_upper(dest, lhs->upper, rhs->lower);
    }
  }
};

static void zero_mul(PBound *dest, const PBound *lhs, const PBound *rhs){

  dest->state = STDBOUND;

  if (roundszero(rhs)){
    PBound temp1 = __EMPTYBOUND;
    PBound temp2 = __EMPTYBOUND;

    mul_lower(&temp1, lhs->lower, rhs->lower);
    mul_lower(&temp2, lhs->upper, rhs->upper);
    mul_upper(&temp1, lhs->lower, rhs->upper);
    mul_upper(&temp2, lhs->upper, rhs->lower);

    dest->lower = (__s(temp1.lower) < __s(temp2.lower)) ? temp1.lower : temp2.lower;
    dest->upper = (__s(temp1.upper) < __s(temp2.upper)) ? temp2.upper : temp1.upper;

  } else if (ispositive(rhs)) {
    mul_lower(dest, lhs->lower, rhs->upper);
    mul_upper(dest, lhs->upper, rhs->upper);
  } else { //#rhs must be negative
    mul_lower(dest, lhs->upper, rhs->lower);
    mul_upper(dest, lhs->lower, rhs->lower);
  }
};

void mul(PBound *dest, const PBound *lhs, const PBound *rhs){

  if (isempty(lhs) || isempty(rhs)) {set_empty(dest); return;}
  if (isallpreals(lhs) || isallpreals(rhs)) {set_allreals(dest); return;}

  if (issingle(lhs)) {single_mul(dest, lhs, rhs); return;}
  if (issingle(rhs)) {single_mul(dest, rhs, lhs); return;}

  if (roundsinf(lhs)) {inf_mul(dest, lhs, rhs); return;}
  if (roundsinf(rhs)) {inf_mul(dest, rhs, lhs); return;}

  if (roundszero(lhs)) {zero_mul(dest, lhs, rhs); return;}
  if (roundszero(rhs)) {zero_mul(dest, rhs, lhs); return;}

  /*handle the case of bounds which stick to their parity*/

  bool lhs_neg = isnegative(lhs);
  bool rhs_neg = isnegative(rhs);

  PTile lhs_lower_proxy = lhs_neg ? tile_additiveinverse(lhs->upper) : lhs->lower;
  PTile lhs_upper_proxy = lhs_neg ? tile_additiveinverse(lhs->lower) : lhs->upper;
  PTile rhs_lower_proxy = rhs_neg ? tile_additiveinverse(rhs->upper) : rhs->lower;
  PTile rhs_upper_proxy = rhs_neg ? tile_additiveinverse(rhs->lower) : rhs->upper;

  //presume our result is going to be a beautiful standard bound (may have to collapse.)
  dest->state = STDBOUND;

  //calculate the lower side.
  mul_lower(dest, lhs_lower_proxy, rhs_lower_proxy);

  //calculate the upper side
  mul_upper(dest, lhs_upper_proxy, rhs_upper_proxy);

  if (lhs_neg ^ rhs_neg) {additiveinverse(dest);};

  //collapse it if we must.
  collapseifsingle(dest);

  return;
}
