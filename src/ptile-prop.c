#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/PTile.h"

bool is_pf_special(PTile x){
  return (x & __mask1) == 0;
}

bool is_pf_normal(PTile x){
  return (x & __mask1) != 0;
}

bool is_pf_positive(PTile x){
  return (is_pf_normal(x)) && ((x & __inf) == 0);
}

bool is_pf_negative(PTile x){
  return (is_pf_normal(x)) && ((x & __inf) != 0);
}

bool is_pf_inverted(PTile x){
  if ((x & __mask2) == 0) {return false;};
  return ((x & __inf) == 0) == ((x & __one) == 0);
}

bool is_pf_inf(PTile x){
  return (x == __inf);
}

bool is_pf_zero(PTile x){
  return (x == __zero);
}

bool is_pf_exact(PTile x){
  return (x & (PENV->increment)) == 0;
}

bool is_pf_ulp(PTile x){
  return (x & (PENV->increment)) != 0;
}
