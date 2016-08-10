#include "../include/pbound.h"
#include "../include/pfloat.h"

void additiveinverse(PBound *v){
  PFloat temp = pf_additiveinverse(v->lower);
  v->lower = pf_additiveinverse(v->upper);
  v->upper = temp;
}

void multiplicativeinverse(PBound *v){
  PFloat temp = pf_multiplicativeinverse(v->lower);
  v->lower = pf_multiplicativeinverse(v->upper);
  v->upper = temp;
};
