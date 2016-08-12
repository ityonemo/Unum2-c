#include "PFloat4-test.h"
#include <stdio.h>

bool test_add(const PBound *lhs, const PBound *rhs, const PBound *res){
  //set up a temporary value to hold the result.
  PBound testres = __EMPTYBOUND;
  //perform the multiplication.
  add(&testres, lhs, rhs);

  //describe(&testres);
  //do the comparison
  return (eq(res, &testres));
}

void PFloat4_addtest(){
  PBound pb_1001_1011 = {pf1001, pf0011, STDBOUND};
  PBound pb_1001_1111 = {pf1001, pf1111, STDBOUND};
  PBound pb_1001_0111 = {pf1001, pf0111, STDBOUND};


  //inf + anything is inf.
  assert(test_add(&pb1000, &pb1000, &pb1000));
  assert(test_add(&pb1000, &pb1001, &pb1000));
  assert(test_add(&pb1000, &pb1010, &pb1000));
  assert(test_add(&pb1000, &pb1011, &pb1000));
  assert(test_add(&pb1000, &pb1100, &pb1000));
  assert(test_add(&pb1000, &pb1101, &pb1000));
  assert(test_add(&pb1000, &pb1110, &pb1000));
  assert(test_add(&pb1000, &pb1111, &pb1000));
  assert(test_add(&pb1000, &pb0000, &pb1000));
  assert(test_add(&pb1000, &pb0001, &pb1000));
  assert(test_add(&pb1000, &pb0010, &pb1000));
  assert(test_add(&pb1000, &pb0011, &pb1000));
  assert(test_add(&pb1000, &pb0100, &pb1000));
  assert(test_add(&pb1000, &pb0101, &pb1000));
  assert(test_add(&pb1000, &pb0110, &pb1000));
  assert(test_add(&pb1000, &pb0111, &pb1000));

  //1001 (-inf, 2) + ...
  assert(test_add(&pb1001, &pb1000, &pb1000));
  assert(test_add(&pb1001, &pb1001, &pb1001));
  assert(test_add(&pb1001, &pb1010, &pb1001));
  assert(test_add(&pb1001, &pb1011, &pb1001));
  assert(test_add(&pb1001, &pb1100, &pb1001));
  assert(test_add(&pb1001, &pb1101, &pb1001));
  assert(test_add(&pb1001, &pb1110, &pb1001));
  assert(test_add(&pb1001, &pb1111, &pb1001));
  assert(test_add(&pb1001, &pb0000, &pb1001));
  assert(test_add(&pb1001, &pb0001, &pb_1001_1011));
  assert(test_add(&pb1001, &pb0010, &pb_1001_1011));
  assert(test_add(&pb1001, &pb0011, &pb_1001_1011));
  assert(test_add(&pb1001, &pb0100, &pb_1001_1011));
  assert(test_add(&pb1001, &pb0101, &pb_1001_1111));
  assert(test_add(&pb1001, &pb0110, &pb_1001_1111));
  assert(test_add(&pb1001, &pb0111, &pb_1001_0111));

  println("hi\n")
}
