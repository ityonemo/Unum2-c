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
  PBound pb_0001_0011 = {pf0001, pf0011, STDBOUND};
  PBound pb_0001_0101 = {pf0001, pf0101, STDBOUND};
  PBound pb_0001_0111 = {pf0001, pf0111, STDBOUND};
  PBound pb_0011_0101 = {pf0011, pf0101, STDBOUND};
  PBound pb_0101_0111 = {pf0101, pf0111, STDBOUND};
  PBound pb_1001_0111 = {pf1001, pf0111, STDBOUND};
  PBound pb_1001_1011 = {pf1001, pf1011, STDBOUND};
  PBound pb_1001_1111 = {pf1001, pf1111, STDBOUND};
  PBound pb_1011_1101 = {pf1011, pf1101, STDBOUND};
  PBound pb_1011_1111 = {pf1011, pf1111, STDBOUND};
  PBound pb_1101_0011 = {pf1101, pf0011, STDBOUND};
  PBound pb_1101_1111 = {pf1101, pf1111, STDBOUND};
  PBound pb_1111_0001 = {pf1111, pf0001, STDBOUND};

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

  //1010 [2] + ...
  assert(test_add(&pb1010, &pb1000, &pb1000));
  assert(test_add(&pb1010, &pb1001, &pb1001));
  assert(test_add(&pb1010, &pb1010, &pb1001));
  assert(test_add(&pb1010, &pb1011, &pb1001));
  assert(test_add(&pb1010, &pb1100, &pb1001));
  assert(test_add(&pb1010, &pb1101, &pb1001));
  assert(test_add(&pb1010, &pb1110, &pb1001));
  assert(test_add(&pb1010, &pb1111, &pb1001));
  assert(test_add(&pb1010, &pb0000, &pb1010));
  assert(test_add(&pb1010, &pb0001, &pb1011));
  assert(test_add(&pb1010, &pb0010, &pb1011));
  assert(test_add(&pb1010, &pb0011, &pb1011));
  assert(test_add(&pb1010, &pb0100, &pb1100));
  assert(test_add(&pb1010, &pb0101, &pb_1101_1111));
  assert(test_add(&pb1010, &pb0110, &pb0000));
  assert(test_add(&pb1010, &pb0111, &pb_0001_0111));

  //1011 (2..1) + ...
  assert(test_add(&pb1011, &pb1000, &pb1000));
  assert(test_add(&pb1011, &pb1001, &pb1001));
  assert(test_add(&pb1011, &pb1010, &pb1001));
  assert(test_add(&pb1011, &pb1011, &pb1001));
  assert(test_add(&pb1011, &pb1100, &pb1001));
  assert(test_add(&pb1011, &pb1101, &pb_1001_1011));
  assert(test_add(&pb1011, &pb1110, &pb_1001_1011));
  assert(test_add(&pb1011, &pb1111, &pb_1001_1011));
  assert(test_add(&pb1011, &pb0000, &pb1011));
  assert(test_add(&pb1011, &pb0001, &pb_1011_1101));
  assert(test_add(&pb1011, &pb0010, &pb_1011_1101));
  assert(test_add(&pb1011, &pb0011, &pb_1011_1111));
  assert(test_add(&pb1011, &pb0100, &pb_1101_1111));
  assert(test_add(&pb1011, &pb0101, &pb_1101_0011));
  assert(test_add(&pb1011, &pb0110, &pb_0001_0011));
  assert(test_add(&pb1011, &pb0111, &pb_0001_0111));

  //1100 [-1] + ...
  assert(test_add(&pb1100, &pb1000, &pb1000));
  assert(test_add(&pb1100, &pb1001, &pb1001));
  assert(test_add(&pb1100, &pb1010, &pb1001));
  assert(test_add(&pb1100, &pb1011, &pb1001));
  assert(test_add(&pb1100, &pb1100, &pb1010));
  assert(test_add(&pb1100, &pb1101, &pb1011));
  assert(test_add(&pb1100, &pb1110, &pb1011));
  assert(test_add(&pb1100, &pb1111, &pb1011));
  assert(test_add(&pb1100, &pb0000, &pb1100));
  assert(test_add(&pb1100, &pb0001, &pb1101));
  assert(test_add(&pb1100, &pb0010, &pb1110));
  assert(test_add(&pb1100, &pb0011, &pb1111));
  assert(test_add(&pb1100, &pb0100, &pb0000));
  assert(test_add(&pb1100, &pb0101, &pb_0001_0011));
  assert(test_add(&pb1100, &pb0110, &pb0100));
  assert(test_add(&pb1100, &pb0111, &pb_0101_0111));

  //1101 (-1 -0.5) + ...
  assert(test_add(&pb1101, &pb1000, &pb1000));
  assert(test_add(&pb1101, &pb1001, &pb1001));
  assert(test_add(&pb1101, &pb1010, &pb1001));
  assert(test_add(&pb1101, &pb1011, &pb_1001_1011));
  assert(test_add(&pb1101, &pb1100, &pb1011));
  assert(test_add(&pb1101, &pb1101, &pb1011));
  assert(test_add(&pb1101, &pb1110, &pb1011));
  assert(test_add(&pb1101, &pb1111, &pb_1011_1101));
  assert(test_add(&pb1101, &pb0000, &pb1101));
  assert(test_add(&pb1101, &pb0001, &pb_1101_1111));
  assert(test_add(&pb1101, &pb0010, &pb1111));
  assert(test_add(&pb1101, &pb0011, &pb_1111_0001));
  assert(test_add(&pb1101, &pb0100, &pb0001));
  assert(test_add(&pb1101, &pb0101, &pb_0001_0101));
  assert(test_add(&pb1101, &pb0110, &pb0101));
  assert(test_add(&pb1101, &pb0111, &pb_0101_0111));

  //1110 [-0.5] + ...
  assert(test_add(&pb1110, &pb1000, &pb1000));
  assert(test_add(&pb1110, &pb1001, &pb1001));
  assert(test_add(&pb1110, &pb1010, &pb1001));
  assert(test_add(&pb1110, &pb1011, &pb_1001_1011));
  assert(test_add(&pb1110, &pb1100, &pb1011));
  assert(test_add(&pb1110, &pb1101, &pb1011));
  assert(test_add(&pb1110, &pb1110, &pb1100));
  assert(test_add(&pb1110, &pb1111, &pb1101));
  assert(test_add(&pb1110, &pb0000, &pb1110));
  assert(test_add(&pb1110, &pb0001, &pb1111));
  assert(test_add(&pb1110, &pb0010, &pb0000));
  assert(test_add(&pb1110, &pb0011, &pb0001));
  assert(test_add(&pb1110, &pb0100, &pb0010));
  assert(test_add(&pb1110, &pb0101, &pb_0011_0101));
  assert(test_add(&pb1110, &pb0110, &pb0101));
  assert(test_add(&pb1110, &pb0111, &pb_0101_0111));

  //1111 (-0.5 0) + ...
  assert(test_add(&pb1111, &pb1000, &pb1000));
  assert(test_add(&pb1111, &pb1001, &pb1001));
  assert(test_add(&pb1111, &pb1010, &pb1001));
  assert(test_add(&pb1111, &pb1011, &pb_1001_1011));
  assert(test_add(&pb1111, &pb1100, &pb1011));
  assert(test_add(&pb1111, &pb1101, &pb_1011_1101));
  assert(test_add(&pb1111, &pb1110, &pb1101));
  assert(test_add(&pb1111, &pb1111, &pb_1101_1111));
  assert(test_add(&pb1111, &pb0000, &pb1111));
  assert(test_add(&pb1111, &pb0001, &pb_1111_0001));
  assert(test_add(&pb1111, &pb0010, &pb0001));
  assert(test_add(&pb1111, &pb0011, &pb_0001_0011));
  assert(test_add(&pb1111, &pb0100, &pb0011));
  assert(test_add(&pb1111, &pb0101, &pb_0011_0101));
  assert(test_add(&pb1111, &pb0110, &pb0101));
  assert(test_add(&pb1111, &pb0111, &pb_0101_0111));

  //0000 [0] + ...
  assert(test_add(&pb0000, &pb1000, &pb1000));
  assert(test_add(&pb0000, &pb1001, &pb1001));
  assert(test_add(&pb0000, &pb1010, &pb1010));
  assert(test_add(&pb0000, &pb1011, &pb1011));
  assert(test_add(&pb0000, &pb1100, &pb1100));
  assert(test_add(&pb0000, &pb1101, &pb1101));
  assert(test_add(&pb0000, &pb1110, &pb1110));
  assert(test_add(&pb0000, &pb1111, &pb1111));
  assert(test_add(&pb0000, &pb0000, &pb0000));
  assert(test_add(&pb0000, &pb0001, &pb0001));
  assert(test_add(&pb0000, &pb0010, &pb0010));
  assert(test_add(&pb0000, &pb0011, &pb0011));
  assert(test_add(&pb0000, &pb0100, &pb0100));
  assert(test_add(&pb0000, &pb0101, &pb0101));
  assert(test_add(&pb0000, &pb0110, &pb0110));
  assert(test_add(&pb0000, &pb0111, &pb0111));

  //0001 (0 1/2) + ...
  assert(test_add(&pb0001, &pb1000, &pb1000));
  assert(test_add(&pb0001, &pb1001, &pb_1001_1011));
  assert(test_add(&pb0001, &pb1010, &pb1011));
  assert(test_add(&pb0001, &pb1011, &pb_1011_1101));
  assert(test_add(&pb0001, &pb1100, &pb1101));
  assert(test_add(&pb0001, &pb1101, &pb_1101_1111));
  assert(test_add(&pb0001, &pb1110, &pb1111));
  assert(test_add(&pb0001, &pb1111, &pb_1111_0001));
  assert(test_add(&pb0001, &pb0000, &pb0001));
  assert(test_add(&pb0001, &pb0001, &pb_0001_0011));
  assert(test_add(&pb0001, &pb0010, &pb0011));
  assert(test_add(&pb0001, &pb0011, &pb_0011_0101));
  assert(test_add(&pb0001, &pb0100, &pb0101));
  assert(test_add(&pb0001, &pb0101, &pb_0101_0111));
  assert(test_add(&pb0001, &pb0110, &pb0111));
  assert(test_add(&pb0001, &pb0111, &pb0111));

  //0010 [1/2] + ...
  assert(test_add(&pb0010, &pb1000, &pb1000));
  assert(test_add(&pb0010, &pb1001, &pb_1001_1011));
  assert(test_add(&pb0010, &pb1010, &pb1011));
  assert(test_add(&pb0010, &pb1011, &pb_1011_1101));
  assert(test_add(&pb0010, &pb1100, &pb1110));
  assert(test_add(&pb0010, &pb1101, &pb1111));
  assert(test_add(&pb0010, &pb1110, &pb0000));
  assert(test_add(&pb0010, &pb1111, &pb0001));
  assert(test_add(&pb0010, &pb0000, &pb0010));
  assert(test_add(&pb0010, &pb0001, &pb0011));
  assert(test_add(&pb0010, &pb0010, &pb0100));
  assert(test_add(&pb0010, &pb0011, &pb0101));
  assert(test_add(&pb0010, &pb0100, &pb0101));
  assert(test_add(&pb0010, &pb0101, &pb_0101_0111));
  assert(test_add(&pb0010, &pb0110, &pb0111));
  assert(test_add(&pb0010, &pb0111, &pb0111));

  //0011 [1/2] + ...
  assert(test_add(&pb0010, &pb1000, &pb1000));
  assert(test_add(&pb0010, &pb1001, &pb_1001_1011));
  assert(test_add(&pb0010, &pb1010, &pb1011));
  assert(test_add(&pb0010, &pb1011, &pb_1011_1101));
  assert(test_add(&pb0010, &pb1100, &pb1110));
  assert(test_add(&pb0010, &pb1101, &pb1111));
  assert(test_add(&pb0010, &pb1110, &pb0000));
  assert(test_add(&pb0010, &pb1111, &pb0001));
  assert(test_add(&pb0010, &pb0000, &pb0010));
  assert(test_add(&pb0010, &pb0001, &pb0011));
  assert(test_add(&pb0010, &pb0010, &pb0100));
  assert(test_add(&pb0010, &pb0011, &pb0101));
  assert(test_add(&pb0010, &pb0100, &pb0101));
  assert(test_add(&pb0010, &pb0101, &pb_0101_0111));
  assert(test_add(&pb0010, &pb0110, &pb0111));
  assert(test_add(&pb0010, &pb0111, &pb0111));

  //0011 (1/2 1) + ...
  assert(test_add(&pb0011, &pb1000, &pb1000));
  assert(test_add(&pb0011, &pb1001, &pb_1001_1011));
  assert(test_add(&pb0011, &pb1010, &pb1011));
  assert(test_add(&pb0011, &pb1011, &pb_1011_1111));
  assert(test_add(&pb0011, &pb1100, &pb1111));
  assert(test_add(&pb0011, &pb1101, &pb_1111_0001));
  assert(test_add(&pb0011, &pb1110, &pb0001));
  assert(test_add(&pb0011, &pb1111, &pb_0001_0011));
  assert(test_add(&pb0011, &pb0000, &pb0011));
  assert(test_add(&pb0011, &pb0001, &pb_0011_0101));
  assert(test_add(&pb0011, &pb0010, &pb0101));
  assert(test_add(&pb0011, &pb0011, &pb0101));
  assert(test_add(&pb0011, &pb0100, &pb0101));
  assert(test_add(&pb0011, &pb0101, &pb_0101_0111));
  assert(test_add(&pb0011, &pb0110, &pb0111));
  assert(test_add(&pb0011, &pb0111, &pb0111));

  //0100 [1] + ...
  assert(test_add(&pb0100, &pb1000, &pb1000));
  assert(test_add(&pb0100, &pb1001, &pb_1001_1011));
  assert(test_add(&pb0100, &pb1010, &pb1100));
  assert(test_add(&pb0100, &pb1011, &pb_1101_1111));
  assert(test_add(&pb0100, &pb1100, &pb0000));
  assert(test_add(&pb0100, &pb1101, &pb0001));
  assert(test_add(&pb0100, &pb1110, &pb0010));
  assert(test_add(&pb0100, &pb1111, &pb0011));
  assert(test_add(&pb0100, &pb0000, &pb0100));
  assert(test_add(&pb0100, &pb0001, &pb0101));
  assert(test_add(&pb0100, &pb0010, &pb0101));
  assert(test_add(&pb0100, &pb0011, &pb0101));
  assert(test_add(&pb0100, &pb0100, &pb0110));
  assert(test_add(&pb0100, &pb0101, &pb0111));
  assert(test_add(&pb0100, &pb0110, &pb0111));
  assert(test_add(&pb0100, &pb0111, &pb0111));

  //0101 (1 2) + ...
  assert(test_add(&pb0101, &pb1000, &pb1000));
  assert(test_add(&pb0101, &pb1001, &pb_1001_1111));
  assert(test_add(&pb0101, &pb1010, &pb_1101_1111));
  assert(test_add(&pb0101, &pb1011, &pb_1101_0011));
  assert(test_add(&pb0101, &pb1100, &pb_0001_0011));
  assert(test_add(&pb0101, &pb1101, &pb_0001_0101));
  assert(test_add(&pb0101, &pb1110, &pb_0011_0101));
  assert(test_add(&pb0101, &pb1111, &pb_0011_0101));
  assert(test_add(&pb0101, &pb0000, &pb0101));
  assert(test_add(&pb0101, &pb0001, &pb_0101_0111));
  assert(test_add(&pb0101, &pb0010, &pb_0101_0111));
  assert(test_add(&pb0101, &pb0011, &pb_0101_0111));
  assert(test_add(&pb0101, &pb0100, &pb0111));
  assert(test_add(&pb0101, &pb0101, &pb0111));
  assert(test_add(&pb0101, &pb0110, &pb0111));
  assert(test_add(&pb0101, &pb0111, &pb0111));

  //0110 [2] + ...
  assert(test_add(&pb0110, &pb1000, &pb1000));
  assert(test_add(&pb0110, &pb1001, &pb_1001_1111));
  assert(test_add(&pb0110, &pb1010, &pb0000));
  assert(test_add(&pb0110, &pb1011, &pb_0001_0011));
  assert(test_add(&pb0110, &pb1100, &pb0100));
  assert(test_add(&pb0110, &pb1101, &pb0101));
  assert(test_add(&pb0110, &pb1110, &pb0101));
  assert(test_add(&pb0110, &pb1111, &pb0101));
  assert(test_add(&pb0110, &pb0000, &pb0110));
  assert(test_add(&pb0110, &pb0001, &pb0111));
  assert(test_add(&pb0110, &pb0010, &pb0111));
  assert(test_add(&pb0110, &pb0011, &pb0111));
  assert(test_add(&pb0110, &pb0100, &pb0111));
  assert(test_add(&pb0110, &pb0101, &pb0111));
  assert(test_add(&pb0110, &pb0110, &pb0111));
  assert(test_add(&pb0110, &pb0111, &pb0111));


  //0111 (2 inf) + ...
  assert(test_add(&pb0111, &pb1000, &pb1000));
  assert(test_add(&pb0111, &pb1001, &pb_1001_0111));
  assert(test_add(&pb0111, &pb1010, &pb_0001_0111));
  assert(test_add(&pb0111, &pb1011, &pb_0001_0111));
  assert(test_add(&pb0111, &pb1100, &pb_0101_0111));
  assert(test_add(&pb0111, &pb1101, &pb_0101_0111));
  assert(test_add(&pb0111, &pb1110, &pb_0101_0111));
  assert(test_add(&pb0111, &pb1111, &pb_0101_0111));
  assert(test_add(&pb0111, &pb0000, &pb0111));
  assert(test_add(&pb0111, &pb0001, &pb0111));
  assert(test_add(&pb0111, &pb0010, &pb0111));
  assert(test_add(&pb0111, &pb0011, &pb0111));
  assert(test_add(&pb0111, &pb0100, &pb0111));
  assert(test_add(&pb0111, &pb0101, &pb0111));
  assert(test_add(&pb0111, &pb0110, &pb0111));
  assert(test_add(&pb0111, &pb0111, &pb0111));
}
