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
