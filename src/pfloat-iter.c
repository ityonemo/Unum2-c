#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"

PFloat next(PFloat x){
  return __p(__u(x) + (PENV->increment));
}

PFloat prev(PFloat x){
  return __p(__u(x) - (PENV->increment));
}

//lub:  Least Upper Bound
PFloat lub(PFloat x){
  return is_pf_ulp(x) ? next(x) : x;
}

//glb:  Greatest Lower Bound
PFloat glb(PFloat x){
  return is_pf_ulp(x) ? prev(x) : x;
}

PFloat upper_ulp(PFloat x){
  return is_pf_ulp(x) ? x : next(x);
}

PFloat lower_ulp(PFloat x){
  return is_pf_ulp(x) ? x : prev(x);
}
