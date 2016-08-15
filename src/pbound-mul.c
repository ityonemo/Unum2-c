#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"
#include <stdio.h>

static bool __resultparity(PFloat lhs, PFloat rhs){
  return is_pf_negative(lhs) ^ is_pf_negative(rhs);
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


static void pf_exact_mul(PBound *dest, PFloat lhs, PFloat rhs){
  TRACK("entering pf_exact_mul...");
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

///////////////////////////////////////////////////////////////////////////////
// INNER, OUTER, UPPER, and LOWER inexact mults.

static PFloat pf_inexact_mul_outer(PFloat lhs, PFloat rhs){

  TRACK("entering pf_inexact_mul_outer...");
  //calculate proper outer bounds.
  PFloat _outer_lhs = is_pf_positive(lhs) ? lub(lhs) : glb(lhs);
  PFloat _outer_rhs = is_pf_positive(rhs) ? lub(rhs) : glb(rhs);

  //create a temporary PBound to hold the prospective value.
  PBound res_temp = __EMPTYBOUND;
  pf_exact_mul(&res_temp, _outer_lhs, _outer_rhs);
  if (issingle(&res_temp)) {return res_temp.lower;};
  return is_pf_positive(res_temp.lower) ? res_temp.upper : res_temp.lower;
}

static PFloat pf_inexact_mul_inner(PFloat lhs, PFloat rhs){
  TRACK("entering pf_inexact_mul_inner...");

  PFloat _inner_lhs = is_pf_positive(lhs) ? glb(lhs) : lub(lhs);
  PFloat _inner_rhs = is_pf_positive(rhs) ? glb(rhs) : lub(rhs);

  //create a temporary PBound to hold the prospective value.
  PBound res_temp = __EMPTYBOUND;
  pf_exact_mul(&res_temp, _inner_lhs, _inner_rhs);

  if (issingle(&res_temp)) {return res_temp.lower;};
  return is_pf_negative(res_temp.lower) ? res_temp.upper : res_temp.lower;
}

static PFloat pf_inexact_mul_lower(PFloat lhs, PFloat rhs){
  TRACK("entering pf_inexact_mul_lower...");

  return upper_ulp(__resultparity(lhs, rhs) ? pf_inexact_mul_outer(lhs, rhs) : pf_inexact_mul_inner(lhs, rhs));
}

static PFloat pf_inexact_mul_upper(PFloat lhs, PFloat rhs){

  TRACK("entering pf_inexact_mul_upper....");

  return lower_ulp(__resultparity(lhs, rhs) ? pf_inexact_mul_inner(lhs, rhs) : pf_inexact_mul_outer(lhs, rhs));
}

////////////////////////////////////////////////////////////////////////////////
// FULL inexact mult.

static void pf_inexact_mul(PBound *dest, PFloat lhs, PFloat rhs){

  TRACK("entering pf_inexact_mul...");

  PFloat _l = pf_inexact_mul_lower(lhs, rhs);
  PFloat _u = pf_inexact_mul_upper(lhs, rhs);
  set_bound(dest, _l, _u);
}

static void mul_pf_single(PBound *dest, PFloat lhs, PFloat rhs)
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

static void mul_lower(PBound *dest, PFloat lhs, PFloat rhs){
  TRACK("entering mul_lower....")
  PBound temp;
  mul_pf_single(&temp, lhs, rhs);
  if (isallpreals(&temp)){
    set_allreals(dest);
  } else {
    dest->lower = temp.lower;

    println("set dest lower:")
    describe(dest);
  }
};

static void mul_upper(PBound *dest, PFloat lhs, PFloat rhs){
  TRACK("entering mul_upper....");
  //bail if this problem is already solved.
  if (isallpreals(dest)) {return;}

  PBound temp;
  mul_pf_single(&temp, lhs, rhs);
  if (isallpreals(&temp)){
    set_allreals(dest);
  } else {
    dest->upper = issingle(&temp) ? temp.lower : temp.upper;

    println("set dest upper:")
    describe(dest);
  }
};

static void single_mul(PBound *dest, const PBound *lhs, const PBound *rhs)
{
  TRACK("entering single_mul...");

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

    bool lhs_neg = is_pf_negative(lhs->lower);
    PFloat lhs_proxy = lhs_neg ? pf_additiveinverse(lhs->lower) : lhs->lower;

    dest->state = STDBOUND;
    mul_lower(dest, rhs->lower, lhs_proxy);
    mul_upper(dest, rhs->upper, lhs_proxy);

    if (lhs_neg) {additiveinverse(dest);};
  }
  collapseifsingle(dest);
};

int muldiv_index(long long lattice1, long long lattice2){
  //matrix points are generated by shifting away the ulp status and subtracting
  //one, because c arrays are zero-indexed.
  int lpoint1 = ((lattice1 >> 1) - 1);
  int lpoint2 = ((lattice2 >> 1) - 1);
  //shift the left value index by the number of lattice bits.
  return (lpoint1 << (PENV->latticebits - 1)) + lpoint2;
}

void inf_mul(PBound *dest, const PBound *lhs, const PBound *rhs)
{
  TRACK("entering inf_mul...")

  if (roundszero(rhs) || is_pf_zero(rhs->lower) || is_pf_zero(rhs->upper)) {
    //the case where the right hand side hits zero:
    set_allreals(dest);
    return;
  } else if (roundszero(lhs)) {
    //the case where the lhs value rounds both infinity and zero.
    //the value for rhs must be a nonflipped doubel interval on one side of zero.

    if (roundsinf(rhs)) {set_allreals(dest); return;}

    int _state = isnegative(lhs) * 1 + isnegative(rhs) * 2;

    PFloat _l, _u;

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

    dest->lower = __s(temp1.lower) < __s(temp2.lower) ? temp1.lower : temp2.lower;
    dest->upper = __s(temp1.upper) < __s(temp2.upper) ? temp1.upper : temp2.upper;

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
      mul_upper(dest, lhs->upper, rhs->upper);
    }
  }
};

static void zero_mul(PBound *dest, const PBound *lhs, const PBound *rhs){

  TRACK("entering zero_mul...")

  if (roundszero(rhs)){
    PBound temp1 = __EMPTYBOUND;
    PBound temp2 = __EMPTYBOUND;

    mul_lower(&temp1, lhs->lower, rhs->lower);
    mul_lower(&temp2, lhs->upper, rhs->upper);
    mul_upper(&temp1, lhs->lower, rhs->upper);
    mul_upper(&temp2, lhs->upper, rhs->lower);

    dest->lower = __s(temp1.lower) < __s(temp2.lower) ? temp1.lower : temp2.lower;
    dest->upper = __s(temp1.upper) < __s(temp2.upper) ? temp1.upper : temp2.upper;
  } else if (ispositive(rhs)) {
    mul_lower(dest, lhs->lower, rhs->upper);
    mul_upper(dest, lhs->upper, rhs->upper);
  } else { //#rhs must be negative
    mul_lower(dest, lhs->upper, rhs->lower);
    mul_upper(dest, lhs->lower, rhs->lower);
  }
};

void mul(PBound *dest, const PBound *lhs, const PBound *rhs){

  TRACK("entering mul...");
  /*first handle all the corner cases*/

  println("a");

  if (isempty(lhs) || isempty(rhs)) {set_empty(dest); return;}
  if (isallpreals(lhs) || isallpreals(rhs)) {set_allreals(dest); return;}

  println("b");

  if (issingle(lhs)) {single_mul(dest, lhs, rhs); return;}
  if (issingle(rhs)) {single_mul(dest, rhs, lhs); return;}

  println("c");

  if (roundsinf(lhs)) {inf_mul(dest, lhs, rhs); return;}
  if (roundsinf(rhs)) {inf_mul(dest, rhs, lhs); return;}

  println("d");

  if (roundszero(lhs)) {zero_mul(dest, lhs, rhs); return;}
  if (roundszero(rhs)) {zero_mul(dest, rhs, lhs); return;}

  println("e");

  /*handle the case of bounds which stick to their parity*/

  bool lhs_neg = isnegative(lhs);
  bool rhs_neg = isnegative(rhs);

  PFloat lhs_lower_proxy = lhs_neg ? pf_additiveinverse(lhs->upper) : lhs->lower;
  PFloat lhs_upper_proxy = lhs_neg ? pf_additiveinverse(lhs->lower) : lhs->upper;
  PFloat rhs_lower_proxy = rhs_neg ? pf_additiveinverse(rhs->upper) : rhs->lower;
  PFloat rhs_upper_proxy = rhs_neg ? pf_additiveinverse(rhs->lower) : rhs->upper;

  //presume our result is going to be a beautiful standard bound (may have to collapse.)
  dest->state = STDBOUND;

  println("lower input values");
  hexprint(lhs_lower_proxy);
  hexprint(lhs_upper_proxy);

  println("upper input values");
  hexprint(rhs_lower_proxy);
  hexprint(rhs_upper_proxy);

  //calculate the lower side.
  mul_lower(dest, lhs_lower_proxy, rhs_lower_proxy);

  println("lower partial");
  describe(dest);

  //calculate the upper side
  mul_upper(dest, lhs_upper_proxy, rhs_upper_proxy);
  println("upper partial");
  describe(dest);

  if (lhs_neg ^ rhs_neg) {additiveinverse(dest);};
  println("flipped:");
  describe(dest);

  //collapse it if we must.
  collapseifsingle(dest);

  return;
}
