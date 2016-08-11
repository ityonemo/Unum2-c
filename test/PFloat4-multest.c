#include "PFloat4-test.h"
#include <stdio.h>

bool test_mul(const PBound *lhs, const PBound *rhs, const PBound *res){
  //set up a temporary value to hold the result.
  PBound testres = __EMPTYBOUND;
  //perform the multiplication.
  mul(&testres, lhs, rhs);

  //describe(&testres);
  //do the comparison
  return (eq(res, &testres));
}


void PFloat4_multest(){
  const PBound allreals = {__zero, __zero, ALLREALS};
  const PBound pb_0001_0011 = {pf0001, pf0011, STDBOUND};
  const PBound pb_0001_0111 = {pf0001, pf0111, STDBOUND};
  const PBound pb_0011_0101 = {pf0011, pf0101, STDBOUND};
  const PBound pb_0101_0111 = {pf0101, pf0111, STDBOUND};
  const PBound pb_1001_1011 = {pf1001, pf1011, STDBOUND};
  const PBound pb_1001_1111 = {pf1001, pf1111, STDBOUND};
  const PBound pb_1011_1101 = {pf1011, pf1101, STDBOUND};
  const PBound pb_1101_1111 = {pf1101, pf1111, STDBOUND};

  //1000 line [inf] * ...
  /*assert(test_mul(&pb1000, &pb1000, &pb1000));
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

  //1001 line (-2->inf) *...
  assert(test_mul(&pb1001, &pb1000, &pb1000));
  assert(test_mul(&pb1001, &pb1001, &pb0111));
  assert(test_mul(&pb1001, &pb1010, &pb0111));
  assert(test_mul(&pb1001, &pb1011, &pb0111));
  assert(test_mul(&pb1001, &pb1100, &pb0111));
  assert(test_mul(&pb1001, &pb1101, &pb_0101_0111));
  assert(test_mul(&pb1001, &pb1110, &pb_0101_0111));
  assert(test_mul(&pb1001, &pb1111, &pb_0001_0111));
  assert(test_mul(&pb1001, &pb0000, &pb0000));
  assert(test_mul(&pb1001, &pb0001, &pb_1001_1111));
  assert(test_mul(&pb1001, &pb0010, &pb_1001_1011));
  assert(test_mul(&pb1001, &pb0011, &pb_1001_1011));
  assert(test_mul(&pb1001, &pb0100, &pb1001));
  assert(test_mul(&pb1001, &pb0101, &pb1001));
  assert(test_mul(&pb1001, &pb0110, &pb1001));
  assert(test_mul(&pb1001, &pb0111, &pb1001));

  //1010 line [-2] *...
  assert(test_mul(&pb1010, &pb1000, &pb1000));
  assert(test_mul(&pb1010, &pb1001, &pb0111));
  assert(test_mul(&pb1010, &pb1010, &pb0111));
  assert(test_mul(&pb1010, &pb1011, &pb0111));
  assert(test_mul(&pb1010, &pb1100, &pb0110));
  assert(test_mul(&pb1010, &pb1101, &pb0101));
  assert(test_mul(&pb1010, &pb1110, &pb0100));
  assert(test_mul(&pb1010, &pb1111, &pb_0001_0011));
  assert(test_mul(&pb1010, &pb0000, &pb0000));
  assert(test_mul(&pb1010, &pb0001, &pb_1101_1111));
  assert(test_mul(&pb1010, &pb0010, &pb1100));
  assert(test_mul(&pb1010, &pb0011, &pb1011));
  assert(test_mul(&pb1010, &pb0100, &pb1010));
  assert(test_mul(&pb1010, &pb0101, &pb1001));
  assert(test_mul(&pb1010, &pb0110, &pb1001));
  assert(test_mul(&pb1010, &pb0111, &pb1001));

  //1011 line (-2 -1) * ...
  assert(test_mul(&pb1011, &pb1000, &pb1000));
  assert(test_mul(&pb1011, &pb1001, &pb0111));
  assert(test_mul(&pb1011, &pb1010, &pb0111));
  assert(test_mul(&pb1011, &pb1011, &pb_0101_0111));
  assert(test_mul(&pb1011, &pb1100, &pb0101));
  assert(test_mul(&pb1011, &pb1101, &pb_0011_0101));
  assert(test_mul(&pb1011, &pb1110, &pb0011));
  assert(test_mul(&pb1011, &pb1111, &pb_0001_0011));
  assert(test_mul(&pb1011, &pb0000, &pb0000));
  assert(test_mul(&pb1011, &pb0001, &pb_1101_1111));
  assert(test_mul(&pb1011, &pb0010, &pb1101));
  assert(test_mul(&pb1011, &pb0011, &pb_1011_1101));
  assert(test_mul(&pb1011, &pb0100, &pb1011));
  assert(test_mul(&pb1011, &pb0101, &pb_1001_1011));
  assert(test_mul(&pb1011, &pb0110, &pb1001));
  assert(test_mul(&pb1011, &pb0111, &pb1001));

  //1100 line [-1] * ...
  assert(test_mul(&pb1100, &pb1000, &pb1000));
  assert(test_mul(&pb1100, &pb1001, &pb0111));
  assert(test_mul(&pb1100, &pb1010, &pb0110));
  assert(test_mul(&pb1100, &pb1011, &pb0101));
  assert(test_mul(&pb1100, &pb1100, &pb0100));
  assert(test_mul(&pb1100, &pb1101, &pb0011));
  assert(test_mul(&pb1100, &pb1110, &pb0010));
  assert(test_mul(&pb1100, &pb1111, &pb0001));
  assert(test_mul(&pb1100, &pb0000, &pb0000));
  assert(test_mul(&pb1100, &pb0001, &pb1111));
  assert(test_mul(&pb1100, &pb0010, &pb1110));
  assert(test_mul(&pb1100, &pb0011, &pb1101));
  assert(test_mul(&pb1100, &pb0100, &pb1100));
  assert(test_mul(&pb1100, &pb0101, &pb1011));
  assert(test_mul(&pb1100, &pb0110, &pb1010));
  assert(test_mul(&pb1100, &pb0111, &pb1001));

  //1101 line (-1 -0.5) * ...
  assert(test_mul(&pb1101, &pb1000, &pb1000));
  assert(test_mul(&pb1101, &pb1001, &pb_0101_0111));
  assert(test_mul(&pb1101, &pb1010, &pb0101));
  assert(test_mul(&pb1101, &pb1011, &pb_0011_0101));
  assert(test_mul(&pb1101, &pb1100, &pb0011));
  assert(test_mul(&pb1101, &pb1101, &pb_0001_0011));
  assert(test_mul(&pb1101, &pb1110, &pb0001));
  assert(test_mul(&pb1101, &pb1111, &pb0001));
  assert(test_mul(&pb1101, &pb0000, &pb0000));
  assert(test_mul(&pb1101, &pb0001, &pb1111));
  assert(test_mul(&pb1101, &pb0010, &pb1111));
  assert(test_mul(&pb1101, &pb0011, &pb_1101_1111));
  assert(test_mul(&pb1101, &pb0100, &pb1101));
  assert(test_mul(&pb1101, &pb0101, &pb_1011_1101));
  assert(test_mul(&pb1101, &pb0110, &pb1011));
  assert(test_mul(&pb1101, &pb0111, &pb_1001_1011));

  //1110 line [-0.5] * ...
  assert(test_mul(&pb1110, &pb1000, &pb1000));
  assert(test_mul(&pb1110, &pb1001, &pb_0101_0111));
  assert(test_mul(&pb1110, &pb1010, &pb0100));
  assert(test_mul(&pb1110, &pb1011, &pb0011));
  assert(test_mul(&pb1110, &pb1100, &pb0010));
  assert(test_mul(&pb1110, &pb1101, &pb0001));
  assert(test_mul(&pb1110, &pb1110, &pb0001));
  assert(test_mul(&pb1110, &pb1111, &pb0001));
  assert(test_mul(&pb1110, &pb0000, &pb0000));
  assert(test_mul(&pb1110, &pb0001, &pb1111));
  assert(test_mul(&pb1110, &pb0010, &pb1111));
  assert(test_mul(&pb1110, &pb0011, &pb1111));
  assert(test_mul(&pb1110, &pb0100, &pb1110));
  assert(test_mul(&pb1110, &pb0101, &pb1101));
  assert(test_mul(&pb1110, &pb0110, &pb1100));
  assert(test_mul(&pb1110, &pb0111, &pb_1001_1011));

  //1111 line (-0.5 0) * ...
  assert(test_mul(&pb1111, &pb1000, &pb1000));
  assert(test_mul(&pb1111, &pb1001, &pb_0001_0111));
  assert(test_mul(&pb1111, &pb1010, &pb_0001_0011));
  assert(test_mul(&pb1111, &pb1011, &pb_0001_0011));
  assert(test_mul(&pb1111, &pb1100, &pb0001));
  assert(test_mul(&pb1111, &pb1101, &pb0001));
  assert(test_mul(&pb1111, &pb1110, &pb0001));
  assert(test_mul(&pb1111, &pb1111, &pb0001));
  assert(test_mul(&pb1111, &pb0000, &pb0000));
  assert(test_mul(&pb1111, &pb0001, &pb1111));
  assert(test_mul(&pb1111, &pb0010, &pb1111));
  assert(test_mul(&pb1111, &pb0011, &pb1111));
  assert(test_mul(&pb1111, &pb0100, &pb1111));
  assert(test_mul(&pb1111, &pb0101, &pb_1101_1111));
  assert(test_mul(&pb1111, &pb0110, &pb_1101_1111));
  assert(test_mul(&pb1111, &pb0111, &pb_1001_1111));*/

  //0000 line [0] * ...
  assert(test_mul(&pb0000, &pb1000, &allreals));
  assert(test_mul(&pb0000, &pb1001, &pb0000));
  assert(test_mul(&pb0000, &pb1010, &pb0000));
  assert(test_mul(&pb0000, &pb1011, &pb0000));
  assert(test_mul(&pb0000, &pb1100, &pb0000));
  assert(test_mul(&pb0000, &pb1101, &pb0000));
  assert(test_mul(&pb0000, &pb1110, &pb0000));
  assert(test_mul(&pb0000, &pb1111, &pb0000));
  assert(test_mul(&pb0000, &pb0000, &pb0000));
  assert(test_mul(&pb0000, &pb0001, &pb0000));
  assert(test_mul(&pb0000, &pb0010, &pb0000));
  assert(test_mul(&pb0000, &pb0011, &pb0000));
  assert(test_mul(&pb0000, &pb0100, &pb0000));
  assert(test_mul(&pb0000, &pb0101, &pb0000));
  assert(test_mul(&pb0000, &pb0110, &pb0000));
  assert(test_mul(&pb0000, &pb0111, &pb0000));
}
