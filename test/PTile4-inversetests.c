#include "PTile4-test.h"

void PTile4_inversetests(){
  assert(pf_additiveinverse(pf1000) == pf1000);
  assert(pf_additiveinverse(pf1001) == pf0111);
  assert(pf_additiveinverse(pf1010) == pf0110);
  assert(pf_additiveinverse(pf1011) == pf0101);
  assert(pf_additiveinverse(pf1100) == pf0100);
  assert(pf_additiveinverse(pf1101) == pf0011);
  assert(pf_additiveinverse(pf1110) == pf0010);
  assert(pf_additiveinverse(pf1111) == pf0001);
  assert(pf_additiveinverse(pf0000) == pf0000);
  assert(pf_additiveinverse(pf0001) == pf1111);
  assert(pf_additiveinverse(pf0010) == pf1110);
  assert(pf_additiveinverse(pf0011) == pf1101);
  assert(pf_additiveinverse(pf0100) == pf1100);
  assert(pf_additiveinverse(pf0101) == pf1011);
  assert(pf_additiveinverse(pf0110) == pf1010);
  assert(pf_additiveinverse(pf0111) == pf1001);

  assert(pf_multiplicativeinverse(pf1000) == pf0000);
  assert(pf_multiplicativeinverse(pf1001) == pf1111);
  assert(pf_multiplicativeinverse(pf1010) == pf1110);
  assert(pf_multiplicativeinverse(pf1011) == pf1101);
  assert(pf_multiplicativeinverse(pf1100) == pf1100);
  assert(pf_multiplicativeinverse(pf1101) == pf1011);
  assert(pf_multiplicativeinverse(pf1110) == pf1010);
  assert(pf_multiplicativeinverse(pf1111) == pf1001);
  assert(pf_multiplicativeinverse(pf0000) == pf1000);
  assert(pf_multiplicativeinverse(pf0001) == pf0111);
  assert(pf_multiplicativeinverse(pf0010) == pf0110);
  assert(pf_multiplicativeinverse(pf0011) == pf0101);
  assert(pf_multiplicativeinverse(pf0100) == pf0100);
  assert(pf_multiplicativeinverse(pf0101) == pf0011);
  assert(pf_multiplicativeinverse(pf0110) == pf0010);
  assert(pf_multiplicativeinverse(pf0111) == pf0001);
}
