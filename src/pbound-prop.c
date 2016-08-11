#include "../include/pbound.h"
#include "../include/pfloat.h"

bool isempty(const PBound *v){
  return v->state == EMPTYSET;
}
bool issingle(const PBound *v){
  return v->state == SINGLETON;
}
bool isbound(const PBound *v){
  return v->state == STDBOUND;
}
bool isallpreals(const PBound *v){
  return v->state == ALLREALS;
}

bool __roundsinf_checked(const PBound *v){
  return (__s(v->upper) < __s(v->lower));
}
bool roundsinf(const PBound *v){
  return (v->state == ALLREALS) || ((v->state == STDBOUND) && __roundsinf_checked(v));
}

bool roundszero(const PBound *v){
  if (v->state == EMPTYSET) {return false;}
  if (v->state == SINGLETON) {return false;}
  if (v->state == ALLREALS) {return true;}

  if (roundsinf(v))
    {return (is_pf_positive(v->lower) && is_pf_positive(v->upper)) || (is_pf_negative(v->lower) && is_pf_negative(v->upper));}
  else
    {return is_pf_negative(v->lower) && is_pf_positive(v->upper);}
}

bool isnegative(const PBound *v){
  return (is_pf_inf(v->lower) || is_pf_negative(v->lower)) &&  (is_pf_negative(v->upper) || is_pf_zero(v->upper));
}

bool ispositive(const PBound *v){
  return (is_pf_zero(v->lower) || is_pf_positive(v->lower)) && (is_pf_positive(v->upper) || is_pf_inf(v->upper));
}

void collapseifsingle(PBound *v){
  if ((v->state == STDBOUND) && (v->upper == v->lower)){
    v->state = SINGLETON;
  };
}
