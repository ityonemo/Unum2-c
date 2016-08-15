#include "PFloat4-test.h"

void PFloat4_paritytests(){
  assert(!is_pf_positive(pf1000));
  assert(!is_pf_positive(pf1001));
  assert(!is_pf_positive(pf1010));
  assert(!is_pf_positive(pf1011));
  assert(!is_pf_positive(pf1100));
  assert(!is_pf_positive(pf1101));
  assert(!is_pf_positive(pf1110));
  assert(!is_pf_positive(pf1111));
  assert(!is_pf_positive(pf0000));
  assert( is_pf_positive(pf0001));
  assert( is_pf_positive(pf0010));
  assert( is_pf_positive(pf0011));
  assert( is_pf_positive(pf0100));
  assert( is_pf_positive(pf0101));
  assert( is_pf_positive(pf0110));
  assert( is_pf_positive(pf0111));

  assert(!is_pf_negative(pf1000));
  assert( is_pf_negative(pf1001));
  assert( is_pf_negative(pf1010));
  assert( is_pf_negative(pf1011));
  assert( is_pf_negative(pf1100));
  assert( is_pf_negative(pf1101));
  assert( is_pf_negative(pf1110));
  assert( is_pf_negative(pf1111));
  assert(!is_pf_negative(pf0000));
  assert(!is_pf_negative(pf0001));
  assert(!is_pf_negative(pf0010));
  assert(!is_pf_negative(pf0011));
  assert(!is_pf_negative(pf0100));
  assert(!is_pf_negative(pf0101));
  assert(!is_pf_negative(pf0110));
  assert(!is_pf_negative(pf0111));

  assert(!is_pf_inverted(pf1000));
  assert(!is_pf_inverted(pf1001));
  assert(!is_pf_inverted(pf1010));
  assert(!is_pf_inverted(pf1011));
  assert(!is_pf_inverted(pf1100));
  assert( is_pf_inverted(pf1101));
  assert( is_pf_inverted(pf1110));
  assert( is_pf_inverted(pf1111));
  assert(!is_pf_inverted(pf0000));
  assert( is_pf_inverted(pf0001));
  assert( is_pf_inverted(pf0010));
  assert( is_pf_inverted(pf0011));
  assert(!is_pf_inverted(pf0100));
  assert(!is_pf_inverted(pf0101));
  assert(!is_pf_inverted(pf0110));
  assert(!is_pf_inverted(pf0111));
}

//spot testing on some bounds properties.
void PFloat4_boundtests(){
  const unsigned long long __ngone = __inf & __one;
  PBound testsubject = {__zero, __zero, EMPTYSET};  //create a temporary holding value.
  /******************************************
    TEST roundsinf
  ******************************************/
  //empty set test.
  assert(!roundsinf(&testsubject));
  //single value tests.
  set_zero(&testsubject);
  assert(!roundsinf(&testsubject));
  set_one(&testsubject);
  assert(!roundsinf(&testsubject));
  set_inf(&testsubject);
  assert(!roundsinf(&testsubject));
  //test extant values.
  set_bound(&testsubject, __ngone, __zero);
  assert(!roundsinf(&testsubject));
  set_bound(&testsubject, __ngone, __one);
  assert(!roundsinf(&testsubject));
  set_bound(&testsubject, __ngone, __inf);
  assert( roundsinf(&testsubject));
  set_bound(&testsubject, __zero, __ngone);
  assert( roundsinf(&testsubject));
  set_bound(&testsubject, __zero, __one);
  assert(!roundsinf(&testsubject));
  set_bound(&testsubject, __zero, __inf);
  assert( roundsinf(&testsubject));
  set_bound(&testsubject, __one, __ngone);
  assert( roundsinf(&testsubject));
  set_bound(&testsubject, __one, __zero);
  assert( roundsinf(&testsubject));
  set_bound(&testsubject, __one, __inf);
  assert( roundsinf(&testsubject));
  set_bound(&testsubject, __inf, __ngone);
  assert( roundsinf(&testsubject));
  set_bound(&testsubject, __inf, __zero);
  assert( roundsinf(&testsubject));
  set_bound(&testsubject, __inf, __one);
  assert( roundsinf(&testsubject));
  //allreals test
  set_allreals(&testsubject);
  assert(roundsinf(&testsubject));
  /******************************************
    TEST roundszero
  ******************************************/
  //empty set test.
  set_empty(&testsubject);
  assert(!roundszero(&testsubject));
  //single value tests.
  set_zero(&testsubject);
  assert(!roundszero(&testsubject));
  set_one(&testsubject);
  assert(!roundszero(&testsubject));
  set_inf(&testsubject);
  assert(!roundszero(&testsubject));
  //test extant values.
  set_bound(&testsubject, __ngone, __zero);
  assert( roundszero(&testsubject));
  set_bound(&testsubject, __ngone, __one);
  assert( roundszero(&testsubject));
  set_bound(&testsubject, __ngone, __inf);
  assert( roundszero(&testsubject));
  set_bound(&testsubject, __zero, __ngone);
  assert( roundszero(&testsubject));
  set_bound(&testsubject, __zero, __one);
  assert( roundszero(&testsubject));
  set_bound(&testsubject, __zero, __inf);
  assert( roundszero(&testsubject));
  set_bound(&testsubject, __one, __ngone);
  assert(!roundszero(&testsubject));
  set_bound(&testsubject, __one, __zero);
  assert( roundszero(&testsubject));
  set_bound(&testsubject, __one, __inf);
  assert(!roundszero(&testsubject));
  set_bound(&testsubject, __inf, __ngone);
  assert(!roundszero(&testsubject));
  set_bound(&testsubject, __inf, __zero);
  assert( roundszero(&testsubject));
  set_bound(&testsubject, __inf, __one);
  assert( roundszero(&testsubject));
  //allreals test
  set_allreals(&testsubject);
  assert(roundszero(&testsubject));
}
