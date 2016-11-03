#include "../include/pbound.h"
#include "../include/PTile.h"

void additiveinverse(PBound *v){
  PTile temp;
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
  PTile temp;
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
