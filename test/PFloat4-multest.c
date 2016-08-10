#include "PFloat4-test.h"
#include <stdio.h>

bool test_mul(const PBound *lhs, const PBound *rhs, const PBound *res){
  //set up a temporary value to hold the result.
  PBound testres = {__zero, __zero, EMPTYSET};
  //perform the multiplication.
  mul(&testres, lhs, rhs);
  //do the comparison
  return (eq(res, &testres));
}


void PFloat4_multest(){
  const PBound allreals = {__zero, __zero, ALLREALS};
  /*1000 line*/
  assert(test_mul(&pb1000, &pb1000, &pb1000));
  assert(test_mul(&pb1000, &pb1001, &pb1000));
  assert(test_mul(&pb1000, &pb1010, &pb1000));
  assert(test_mul(&pb1000, &pb1011, &pb1000));
  assert(test_mul(&pb1000, &pb1100, &pb1000));
  assert(test_mul(&pb1000, &pb1101, &pb1000));
  assert(test_mul(&pb1000, &pb1110, &pb1000));
  assert(test_mul(&pb1000, &pb1111, &pb1000));
  assert(test_mul(&pb1000, &pb0000, &allreals));
  assert(test_mul(&pb1000, &pb0001, &pb1000));
  assert(test_mul(&pb1000, &pb0010, &pb1000));
  assert(test_mul(&pb1000, &pb0011, &pb1000));
  assert(test_mul(&pb1000, &pb0100, &pb1000));
  assert(test_mul(&pb1000, &pb0101, &pb1000));
  assert(test_mul(&pb1000, &pb0110, &pb1000));
  assert(test_mul(&pb1000, &pb0111, &pb1000));

  /*1001 line*/
  const PBound pb_0001_0111 = {pf0001, pf0111, ALLREALS};
  const PBound pb_1001_1111 = {pf1001, pf1111, ALLREALS};

  assert(test_mul(&pb1001, &pb1000, &pb0111));
  assert(test_mul(&pb1001, &pb1001, &pb0111));
  assert(test_mul(&pb1001, &pb1010, &pb0111));
  assert(test_mul(&pb1001, &pb1011, &pb0111));
  assert(test_mul(&pb1001, &pb1100, &pb0111));
  assert(test_mul(&pb1001, &pb1101, &pb0111));
  assert(test_mul(&pb1001, &pb1110, &pb0111));
  assert(test_mul(&pb1001, &pb1111, &pb_0001_0111));
  assert(test_mul(&pb1001, &pb0000, &pb0000));
  assert(test_mul(&pb1001, &pb0001, &pb_1001_1111));
  assert(test_mul(&pb1001, &pb0010, &pb1001));
  assert(test_mul(&pb1001, &pb0011, &pb1001));
  assert(test_mul(&pb1001, &pb0100, &pb1001));
  assert(test_mul(&pb1001, &pb0101, &pb1001));
  assert(test_mul(&pb1001, &pb0110, &pb1001));
  assert(test_mul(&pb1001, &pb0111, &pb1001));
}
