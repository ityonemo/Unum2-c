#include "../include/pbound.h"
#include "../include/pfloat.h"

void additiveinverse(PBound *v){
  PFloat temp;
  switch (v->state){
    case SINGLETON:
      v->lower = pf_additiveinverse(v->lower);
    break;
    case STDBOUND:
      temp = pf_additiveinverse(v->lower);
      v->lower = pf_additiveinverse(v->upper);
      v->upper = temp;
    break;
    default:
    break;
  }
}

void multiplicativeinverse(PBound *v){
  PFloat temp;
  switch (v->state){
    case SINGLETON:
      v->lower = pf_multiplicativeinverse(v->lower);
    break;
    case STDBOUND:
      temp = pf_multiplicativeinverse(v->lower);
      v->lower = pf_multiplicativeinverse(v->upper);
      v->upper = temp;
    break;
    default:
    break;
  }
};
