#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/PTile.h"

PTile pf_additiveinverse(PTile value){
  return __p(-__s(value));
}

PTile pf_multiplicativeinverse(PTile value){
  return __p(-__s(value) + __inf);
}
