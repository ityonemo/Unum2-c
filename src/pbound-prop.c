#include "../include/pbound.h"
#include "../include/pfloat.h"

bool isempty(PBound *v){
  return v->state == EMPTYSET;
}
bool issingle(PBound *v){
  return v->state == SINGLETON;
}
bool isbound(PBound *v){
  return v->state == STDBOUND;
}
bool isallpreals(PBound *v){
  return v->state == ALLREALS;
}

bool __roundsinf_checked(PBound *v){
  return (__s(v->upper) < __s(v->lower));
}
bool roundsinf(PBound *v){
  return (v->state == ALLREALS) || ((v->state == STDBOUND) && __roundsinf_checked(v));
}

bool roundszero(PBound *v){
}

bool isnegative(PBound *v){
}

bool ispositive(PBound *v){
}
