#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

PTile pf_synth(bool negative, bool inverted, long long epoch, unsigned long long lattice){
  //check for overflow condition.
  if (epoch > ((1 << (PENV->epochbits - 1)) - 1)){
    if (negative){
      return (inverted ? __nfew : __nmany);
    } else {
      return (inverted ? __few : __many);
    }
  }

  PTile temp = __p(((epoch << PENV->latticebits | lattice) * (PENV->increment)) | __one);
  if (inverted) {temp = tile_multiplicativeinverse(temp);};
  if (negative) {temp = tile_additiveinverse(temp);};
  return temp;
}

long long pf_epoch(PTile value){
  bool flipsign = (value & __one) == 0;
  long long temp = __s(value) * (flipsign ? -1 : 1);
  return (temp & __mask2) >> (63 - PENV->epochbits);
}

unsigned long long pf_lattice(PTile value){
  bool flipsign = is_pf_negative(value) ^ is_pf_inverted(value);
  long long temp = __s(value) * (flipsign ? -1 : 1);
  long long mask = (1 << PENV->latticebits) - 1;
  return ((temp) >> (63 - PENV->epochbits - PENV->latticebits)) & mask;
}
