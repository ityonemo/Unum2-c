#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"

PTile tile_additiveinverse(PTile value){
  return __p(-__s(value));
}

PTile tile_multiplicativeinverse(PTile value){
  return __p(-__s(value) + __inf);
}

PTile tile_abs(PTile value){
  return (__s(value) < 0) ? tile_additiveinverse(value) : value;
}
