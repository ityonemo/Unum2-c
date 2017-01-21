#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

//a couple of helper functions that we'll need.

static void dc_lvdn(__dc_tile *value){
  if (value->lattice == 0){
    if (value->epoch == 0){
      value->lattice = 1;
      value->inverted = !value->inverted;
    } else {
      value->lattice = (1 << PENV->latticebits) - 1;
      value->epoch -= 1;
    }
  } else {
    value->lattice -= 1;
  }
}

static void dc_lvup(__dc_tile * value){
  unsigned long long max_lattice = (1 << PENV->latticebits) - 1;
  if (value->lattice == max_lattice){
    value->lattice = 0;
    value->epoch += 1;
  } else {
    value->lattice += 1;
  }
}

static void dc_lub(__dc_tile *value) {
  if ((value->lattice % 2) != 0) {
    if (value->negative ^ value->inverted) {
      dc_lvup(value);
    } else {
      value->lattice -= 1;
    }
  }
}

static void dc_glb(__dc_tile *value){
  if ((value->lattice % 2) != 0) {
    if (value->negative ^ value->inverted) {
      value->lattice -= 1;
    } else {
      dc_lvup(value);
    }
  }
}

static bool dc_is_additive_inverse(__dc_tile *a, __dc_tile *b){
  if (a->negative == b->negative) {return false;}
  if (a->inverted != b->inverted) {return false;}
  if (a->epoch != b->epoch)       {return false;}
  if (a->lattice != b->lattice)   {return false;}
  return true;
}

static void dc_lower_ulp(__dc_tile *value){
  if ((value->lattice % 2) == 0){
    if (value->negative ^ value->inverted) {
      value->lattice += 1;
    } else {
      dc_lvdn(value);
    }
  }
}

static void dc_upper_ulp(__dc_tile *value) {
  if ((value->lattice % 2) == 0){
    if (value->negative ^ value->inverted) {
      dc_lvdn(value);
    } else {
      value->lattice += 1;
    }
  }
}


PTile exact_fma(PTile a, PTile b, PTile c, bool upper){

  //put onto a stack a decomposed result placeholder.
  __dc_tile dc_result;

  dc_mul(&dc_result, a, b);

  DECOMPOSE(c)

  //next check to see if the result is exact.

  if (dc_result.lattice % 2 == 0){
    //consider swapping the values.

    //next check to see if they are additive inverses.
    if (dc_is_additive_inverse(&dc_result, &c_dc)) { return __zero;}

    return dc_arithmetic_addition(&dc_result, &c_dc);

  } else if (upper) {
    dc_lub(&dc_result);
    if (dc_is_additive_inverse(&dc_result, &c_dc)) { return __nfew;}
    dc_arithmetic_addition(&dc_result, &c_dc);
    dc_lower_ulp(&dc_result);

  } else {
    dc_glb(&dc_result);
    if (dc_is_additive_inverse(&dc_result, &c_dc)) { return __few;}
    dc_arithmetic_addition(&dc_result, &c_dc);
    dc_upper_ulp(&dc_result);
  }

  return tile_synth(&dc_result);
}

PTile inexact_fma(PTile a, PTile b, PTile c, bool upper){
  if (upper){
    bool mul_res_sign = is_tile_negative(a) ^ is_tile_negative(b);
    // if the result is negative, the upper value will result from the inner values for both
    // if the result is positive, the upper value will result from the outer values for both.
    PTile a_bound = (mul_res_sign ^ is_tile_negative(a)) ? glb(a) : lub(a);
    PTile b_bound = (mul_res_sign ^ is_tile_negative(b)) ? glb(b) : lub(b);

    return lower_ulp(exact_fma(a_bound, b_bound, lub(c), true));
  } else {
    //decide if the multiplication result will be positive or negative.
    bool mul_res_sign = is_tile_negative(a) ^ is_tile_negative(b);
    //if the result is negative, the lower value will be the outer values for both
    //if the result is positive, the lower value will result from the inner values for both.
    PTile a_bound = (mul_res_sign ^ is_tile_negative(a)) ? lub(a) : glb(a);
    PTile b_bound = (mul_res_sign ^ is_tile_negative(b)) ? lub(b) : glb(b);

    return upper_ulp(exact_fma(a_bound, b_bound, glb(c), false));
  }
}

PTile tile_fma(PTile a, PTile b, PTile c, bool upper){
  //fma for tile values.

  //go through the list of special values.
  if      (is_tile_inf(a))     {return __inf;}
  else if (is_tile_inf(b))     {return __inf;}
  else if (is_tile_inf(c))     {return __inf;}
  else if (is_tile_zero(a))    {return c;}
  else if (is_tile_zero(b))    {return c;}
  else if (is_tile_zero(c))    {return tile_mul(a, b, upper);}
  else if (is_tile_one(a))     {return tile_add(b, c, upper);}
  else if (is_tile_one(b))     {return tile_add(a, c, upper);}
  else if (is_tile_neg_one(a)) {return tile_add(c, -b, upper);}
  else if (is_tile_neg_one(b)) {return tile_add(c, -a, upper);}

  if (is_tile_exact(a) && is_tile_exact(b) && is_tile_exact(c))
    { return exact_fma(a, b, c, upper); }
  else
    { return inexact_fma(a, b, c, upper); }
}

//inf fma - fused multiply add when the first multiply term contains infinity.
void inf_fma(PBound *res, const PBound *a, const PBound *b, const PBound *c){
  res->state = STDBOUND;

  if (containszero(b))
    //check to see if the rhs contains zero in any way, which will instantly
    //trigger the result to be all projective reals.
    set_allreals(res);
    return;
  if (containszero(a)){
    //it's possible for the value to round both infinity AND zero.

    //check if we have an infinite-valued rhs, which triggers preals.
    if (containsinf(b)) {set_allreals(res); return;}

    //at this juncture, the value a must round both zero and infinity, and
    //the value rhs must be a standard, nonflipped double interval that is only on
    //one side of zero.

    int _state = is_tile_negative(a->lower) * 1 + is_tile_negative(b->lower) * 2;

    PTile c_upper_proxy = issingle(c) ? c->lower : c->upper;

    //assign upper and lower values based on the bounds
    if (_state == 0){
      res->lower = tile_fma(a->lower, b->lower, c->lower     , false);
      res->upper = tile_fma(a->upper, b->upper, c_upper_proxy, true );
    } else if (_state == 1){
      res->lower = tile_fma(a->upper, b->lower, c->lower     , false);
      res->upper = tile_fma(a->lower, b->upper, c_upper_proxy, true );
    } else if (_state == 2){
      res->lower = tile_fma(a->upper, b->lower, c->lower     , false);
      res->upper = tile_fma(a->lower, b->upper, c_upper_proxy, true );
    } else { //if state == 3
      res->lower = tile_fma(a->upper, b->upper, c->lower     , false);
      res->upper = tile_fma(a->lower, b->lower, c_upper_proxy, true );
    }

    // need to check if the result has "flipped around" and now need to be
    // represented by all reals.
    if (__s(prev(res->lower)) <= __s(res->upper)) {
      set_allreals(res);
    }

    return;
  }
  if (containsinf(b)){  //handle the case if rhs rounds infinity.
    //like the double "rounds zero" case, we have to check four possible endpoints.
    bool zero_check = containszero(b) || containsinf(c);

    PTile c_upper_proxy = issingle(c) ? c->lower : c->upper;

    PTile _l1 = is_tile_inf(a->lower) | is_tile_inf(b->lower) ? __inf : tile_fma(a->lower, b->lower, c->lower,      false);
    PTile _l2 = is_tile_inf(a->upper) | is_tile_inf(b->upper) ? __inf : tile_fma(a->upper, b->upper, c->lower,      false);
    PTile _u1 = is_tile_inf(a->lower) | is_tile_inf(b->upper) ? __inf : tile_fma(a->lower, b->upper, c_upper_proxy, true );
    PTile _u2 = is_tile_inf(a->upper) | is_tile_inf(b->lower) ? __inf : tile_fma(a->upper, b->lower, c_upper_proxy, true );

    //construct the result.
    res->lower = (__s(_l1) < __s(_l2)) ? _l1 : _l2;
    res->upper = (__s(_l1) < __s(_l2)) ? _l2 : _l1;

    //check for wraparound to allreals.
    if ((zero_check) && (__s(prev(res->lower)) <= __s(res->upper)) ){
      set_allreals(res);
    }

    return;
  }
  //the last case is if a rounds infinity but b is a "well-behaved" value.
  //canonical example:

  PTile c_upper_proxy = issingle(c) ? c->lower : c->upper;

  if (is_tile_negative(b->lower)){
    res->lower = is_tile_inf(a->upper) ? __inf : tile_fma(a->upper, b->upper, c->lower,      false);
    res->upper = is_tile_inf(a->lower) ? __inf : tile_fma(a->lower, b->upper, c_upper_proxy, true);
  } else {
    res->lower = is_tile_inf(a->lower) ? __inf : tile_fma(a->lower, b->lower, c->lower,      false);
    res->upper = is_tile_inf(a->upper) ? __inf : tile_fma(a->upper, b->lower, c_upper_proxy, true);
  }

  collapseifsingle(res);
}

//zero fma - fused multiply add when the first multiply term is zero.  multiply
//terms are guaranteed not to cross infinity.
void zero_fma(PBound *res, const PBound *a, const PBound *b, const PBound *c){
  res->state = STDBOUND;

  PTile c_upper_proxy = issingle(c) ? c->lower : c->upper;

  if (roundszero(b)){
    // when rhs spans zero, we have to check four possible endpoints.
    res->lower = min(tile_fma(a->lower, b->upper, c->lower     , false), tile_fma(a->upper, b->lower, c->lower     , false));
    res->upper = max(tile_fma(a->lower, b->lower, c_upper_proxy, true ), tile_fma(a->upper, b->upper, c_upper_proxy, true ));
  } else if (is_tile_positive(b->lower)){
    // in the case where the rhs doesn't span zero, we must only multiply by the
    // extremum.
    res->lower = tile_fma(a->lower, b->upper, c->lower     , false);
    res->upper = tile_fma(a->upper, b->upper, c_upper_proxy, true );
  } else {
    res->lower = tile_fma(a->upper, b->lower, c->lower     , false);
    res->upper = tile_fma(a->lower, b->lower, c_upper_proxy, true );
  }

  collapseifsingle(res);
}

void std_fma(PBound *res, const PBound *a, const PBound *b, const PBound *c){
  res->state = STDBOUND;
  // decide if the multiplication result will be positive or negative.
  bool mul_res_sign = is_tile_negative(a->lower) ^ is_tile_negative(b->lower);
  //if the result is negative, the lower value will be the outer values for both
  //if the result is positive, the lower value will result from the inner values for both.
  PTile a_lower_component = ((mul_res_sign ^ is_tile_negative(a->lower)) != 0) ? (a->upper) : (a->lower);
  PTile a_upper_component = ((mul_res_sign ^ is_tile_negative(a->lower)) != 0) ? (a->lower) : (a->upper);
  PTile b_lower_component = ((mul_res_sign ^ is_tile_negative(b->lower)) != 0) ? (b->upper) : (b->lower);
  PTile b_upper_component = ((mul_res_sign ^ is_tile_negative(b->lower)) != 0) ? (b->lower) : (b->upper);

  PTile c_upper_proxy = issingle(c) ? c->lower : c->upper;

  res->lower = tile_fma(a_lower_component, b_lower_component, c->lower, false);
  res->upper = tile_fma(a_upper_component, b_upper_component, c_upper_proxy, true);

  collapseifsingle(res);
}

void pfma(PBound *res, const PBound *a, const PBound *b, const PBound *c){
  //perfoms fma(a,b,c) == a * b + c

  //terminate early on special values.

  if (isempty(a) || isempty(b) || isempty(c)) {set_empty(res); return;}
  if (isallpreals(a) || isallpreals(b) || isallpreals(c)) {set_allreals(res); return;}

  // if c contains inf, it may result in "erasure" of wraparound property from
  // a multiplied a/b result; this interferes with the "broken down" way that
  // the current fma algorithm performs fma.
  if (roundsinf(c) && (roundsinf(a) || roundsinf(b))) {
    //do a multiplication test.
    mul(res, a, b);
    if (isallpreals(res)) {return;}
  }

  //suboperation chain.
  if (issingle(a))        {single_fma(res, a, b, c);}
  else if (issingle(b))   {single_fma(res, b, a, c);}
  else if (roundsinf(a))  {inf_fma(res, a, b, c);}
  else if (roundsinf(b))  {inf_fma(res, b, a, c);}
  else if (roundszero(a)) {zero_fma(res, a, b, c);}
  else if (roundszero(b)) {zero_fma(res, b, a, c);}
  else                    {std_fma(res, b, a, c);}
}
