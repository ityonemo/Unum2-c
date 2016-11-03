#include "PTile4-test.h"
#include <stdio.h>

void PTile4_synthtests(){
  assert(pf_synth(true,  false, 0, 0x0000000000000003) == pf1001);
  assert(pf_synth(true,  false, 0, 0x0000000000000002) == pf1010);
  assert(pf_synth(true,  false, 0, 0x0000000000000001) == pf1011);
  assert(pf_synth(true,  false, 0, 0x0000000000000000) == pf1100);
  assert(pf_synth(true,  true,  0, 0x0000000000000000) == pf1100); //show that -1 can be synthesized either way.
  assert(pf_synth(true,  true,  0, 0x0000000000000001) == pf1101);
  assert(pf_synth(true,  true,  0, 0x0000000000000002) == pf1110);
  assert(pf_synth(true,  true,  0, 0x0000000000000003) == pf1111);
  assert(pf_synth(false, true,  0, 0x0000000000000003) == pf0001);
  assert(pf_synth(false, true,  0, 0x0000000000000002) == pf0010);
  assert(pf_synth(false, true,  0, 0x0000000000000001) == pf0011);
  assert(pf_synth(false, true,  0, 0x0000000000000000) == pf0100);
  assert(pf_synth(false, false, 0, 0x0000000000000000) == pf0100); //show that +1 can be synthesized either way (inverted or not.)
  assert(pf_synth(false, false, 0, 0x0000000000000001) == pf0101);
  assert(pf_synth(false, false, 0, 0x0000000000000002) == pf0110);
  assert(pf_synth(false, false, 0, 0x0000000000000003) == pf0111);

  assert(pf_epoch(pf1001) == 0);
  assert(pf_epoch(pf1010) == 0);
  assert(pf_epoch(pf1011) == 0);
  assert(pf_epoch(pf1100) == 0);
  assert(pf_epoch(pf1101) == 0);
  assert(pf_epoch(pf1110) == 0);
  assert(pf_epoch(pf1111) == 0);
  assert(pf_epoch(pf0001) == 0);
  assert(pf_epoch(pf0010) == 0);
  assert(pf_epoch(pf0011) == 0);
  assert(pf_epoch(pf0100) == 0);
  assert(pf_epoch(pf0101) == 0);
  assert(pf_epoch(pf0110) == 0);
  assert(pf_epoch(pf0111) == 0);

  assert(pf_lattice(pf1001) == 0x0000000000000003);
  assert(pf_lattice(pf1010) == 0x0000000000000002);
  assert(pf_lattice(pf1011) == 0x0000000000000001);
  assert(pf_lattice(pf1100) == 0x0000000000000000);
  assert(pf_lattice(pf1101) == 0x0000000000000001);
  assert(pf_lattice(pf1110) == 0x0000000000000002);
  assert(pf_lattice(pf1111) == 0x0000000000000003);
  assert(pf_lattice(pf0001) == 0x0000000000000003);
  assert(pf_lattice(pf0010) == 0x0000000000000002);
  assert(pf_lattice(pf0011) == 0x0000000000000001);
  assert(pf_lattice(pf0100) == 0x0000000000000000);
  assert(pf_lattice(pf0101) == 0x0000000000000001);
  assert(pf_lattice(pf0110) == 0x0000000000000002);
  assert(pf_lattice(pf0111) == 0x0000000000000003);
}
