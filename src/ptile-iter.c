#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"

PTile next(PTile x){
  return __p(__u(x) + (PENV->increment));
}

PTile prev(PTile x){
  return __p(__u(x) - (PENV->increment));
}

//glb:  Greatest Lower Bound
PTile glb(PTile x){
  return is_pf_ulp(x) ? prev(x) : x;
}

//lub:  Least Upper Bound
PTile lub(PTile x){
  return is_pf_ulp(x) ? next(x) : x;
}


PTile upper_ulp(PTile x){
  return is_pf_ulp(x) ? x : next(x);
}

PTile lower_ulp(PTile x){
  return is_pf_ulp(x) ? x : prev(x);
}
