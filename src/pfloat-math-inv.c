#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"

PFloat pf_additiveinverse(PFloat value){
  return __p(-__s(value));
}

PFloat pf_multiplicativeinverse(PFloat value){
  return __p(-__s(value) + __inf);
}
