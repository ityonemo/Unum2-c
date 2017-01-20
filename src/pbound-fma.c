/*
PTile tile_fma(PTile a, PTile b, PTile c, bool upper){
  //fma for tile values.

  //go through the list of special values.
  if      (is_pf_inf(a))  {return __inf;}
  else if (is_pf_inf(b))  {return __inf;}
  else if (is_pf_inf(c))  {return __inf;}
  else if (is_pf_zero(a)) {return c;}
  else if (is_pf_zero(b)) {return c;}
  else if (is_zero(c))    {return mul(a, b, upper);}
  else if (is_one(a))     {return add(b, c, upper);}
  else if (is_one(b))     {return add(a, c, upper);}
  else if (is_neg_one(a)) {return add(c, -b, upper);}
  else if (is_neg_one(b)) {return add(c, -a, upper);}

  if (isexact(a) && isexact(b) && isexact(c))
    { return exact_fma(a, b, c, upper); }
  else
    { return inexact_fma(a, b, c, upper); }
}

PTile lower_fma(left_tile, right_tile, add_tile){
  return tile_fma(left_tile, right_tile, add_tile, false);
}
PTile upper_fma(left_tile, right_tile, add_tile){
  return tile_fma(left_tile, right_tile, add_tile, true);
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
    if (containsinf(b)) {set_allreals!(res); return;}

    //at this juncture, the value a must round both zero and infinity, and
    //the value rhs must be a standard, nonflipped double interval that is only on
    //one side of zero.

    int _state = is_pf_negative(a->lower) * 1 + is_pf_negative(b->lower) * 2;

    PTile c_upper_proxy = issingle(c) ? c->lower : c->upper;

    //assign upper and lower values based on the bounds
    if (_state == 0){
      res->lower = lower_fma(a->lower, b->lower, c->lower);
      res->upper = upper_fma(a->upper, b->upper, c_upper_proxy);
    } else if (_state == 1){
      res->lower = lower_fma(a->upper, b->lower, c->lower);
      res->upper = upper_fma(a->lower, b->upper, c_upper_proxy);
    } else if (_state == 2){
      res->lower = lower_fma(a->upper, b->lower, c->lower);
      res->upper = upper_fma(a->lower, b->upper, c_upper_proxy);
    } else { //if state == 3
      res->lower = lower_fma(a->upper, b->upper, c->lower);
      res->upper = upper_fma(a->lower, b->lower, c_upper_proxy);
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

    PTile _l1 = is_pf_inf(a->lower) | is_pf_inf(b->lower) ? __inf : lower_fma(a->lower, b->lower, c->lower);
    PTile _l2 = is_pf_inf(a->upper) | is_pf_inf(b->upper) ? __inf : lower_fma(a->upper, b->upper, c->lower);
    PTile _u1 = is_pf_inf(a->lower) | is_pf_inf(b->upper) ? __inf : upper_fma(a->lower, b->upper, c_upper_proxy);
    PTile _u2 = is_pf_inf(a->upper) | is_pf_inf(b->lower) ? __inf : upper_fma(a->upper, b->lower, c_upper_proxy);

    //construct the result.
    res->lower = (__s(_l1) < __s(_l2)) ? _l1 : _l2
    res->upper = (__s(_l1) < __s(_l2)) ? _l2 : _l1

    //check for wraparound to allreals.
    if (zero_check) && (__s(prev(res->lower)) <= __s(res->upper)) {
      set_allreals(res);
    }

    return;
  }
  //the last case is if a rounds infinity but b is a "well-behaved" value.
  //canonical example:

  PTile c_upper_proxy = issingle(c) ? c->lower : c->upper

  if (is_pf_negative(b)){
    res->lower = is_pf_inf(a->upper) ? __inf : lower_fma(a->upper, b->upper, c->lower);
    res->upper = is_pf_inf(a->lower) ? __inf : upper_fma(a->lower, b->upper, c_upper_proxy);
  } else {
    res->lower = is_pf_inf(a->lower) ? __inf : lower_fma(a->lower, b->lower, c->lower);
    res->upper = is_pf_inf(a->upper) ? __inf : upper_fma(a->upper, b->lower, c_upper_proxy);
  }

  collapseifsingle(res);
}

//zero fma - fused multiply add when the first multiply term is zero.  multiply
//terms are guaranteed not to cross infinity.
void zero_fma(PBound *res, const PBound *a, const PBound *b, const PBound *c){
  res->state = STDBOUND;

  c_upper_proxy = issingle(c) ? c->lower : c->upper;

  if (roundszero(b)){
    // when rhs spans zero, we have to check four possible endpoints.
    res->lower = min(lower_fma(a->lower, b->upper, c->lower), lower_fma(a->upper, b->lower, c->lower));
    res->upper = max(upper_fma(a->lower, b->lower, c_upper_proxy), upper_fma(a->upper, b->upper, c_upper_proxy));
  } else if (ispositive(b->lower)){
    // in the case where the rhs doesn't span zero, we must only multiply by the
    // extremum.
    res->lower = lower_fma(a->lower, b->upper, c->lower);
    res->upper = upper_fma(a->upper, b->upper, c_upper_proxy);
  } else {
    res->lower = lower_fma(a->upper, b->lower, c->lower);
    res->upper = upper_fma(a->lower, b->lower, c_upper_proxy);
  }

  collapseifsingle(res);
}

void std_fma(PBound *res, const PBound *a, const PBound *b, const PBound *c){
  res->state = STDBOUND;
  // decide if the multiplication result will be positive or negative.
  bool mul_res_sign = is_pf_negative(a->lower) ^ is_pf_negative(b->lower)
  //if the result is negative, the lower value will be the outer values for both
  //if the result is positive, the lower value will result from the inner values for both.
  a_lower_component = ((mul_res_sign ^ is_pf_negative(a->lower)) != 0) ? (a->upper) : (a->lower)
  a_upper_component = ((mul_res_sign ^ is_pf_negative(a->lower)) != 0) ? (a->lower) : (a->upper)
  b_lower_component = ((mul_res_sign ^ is_pf_negative(b->lower)) != 0) ? (b->upper) : (b->lower)
  b_upper_component = ((mul_res_sign ^ is_pf_negative(b->lower)) != 0) ? (b->lower) : (b->upper)

  PTile c_upper_proxy = issingle(c) ? c->lower : c->upper

  res->lower = tile_fma(a_lower_component, b_lower_component, c->lower)
  res->upper = tile_fma(a_upper_component, b_upper_component, c_upper_proxy)

  collapseifsingle(res);
}

void fma(PBound *res, const PBound *a, const PBound *b, const PBound *c){
*/
  /*perfoms fma(a,b,c) == a * b + c */
/*
  //terminate early on special values.

  if (isempty(a) || isempty(b) || isempty(c)) {set_empty!(res); return;}
  if (isallpreals(a) || isallpreals(b) || isallpreals(c)) {set_preals!(res); return;}

  // if c contains inf, it may result in "erasure" of wraparound property from
  // a multiplied a/b result; this interferes with the "broken down" way that
  // the current fma algorithm performs fma.
  if (roundsinf(c) && (roundsinf(a) || roundsinf(b))) {
    //do a multiplication test.
    mul(res, a, b)
    isallpreals(res) && return
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
*/
