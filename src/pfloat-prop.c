#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"

bool is_pf_special(PFloat x){
  return (x & __mask1) == 0;
}

bool is_pf_normal(PFloat x){
  return (x & __mask1) != 0;
}

bool is_pf_positive(PFloat x){
  return (is_pf_normal(x)) && ((x & __inf) == 0);
}

bool is_pf_negative(PFloat x){
  return (is_pf_normal(x)) && ((x & __inf) != 0);
}

bool is_pf_inverted(PFloat x){
  if ((x & __mask2) == 0) {return false;};
  return ((x & __inf) == 0) == ((x & __one) == 0);
}

bool is_pf_exact(PFloat x){
  return (x & (PENV->increment)) == 0;
}

bool is_pf_ulp(PFloat x){
  return (x & (PENV->increment)) != 0;
}
