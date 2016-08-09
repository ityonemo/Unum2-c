
#include "PFloat4-test.h"
#include "../src/PFloat4.h"

const PFloat pf0000 = __p(0x0000000000000000);
const PFloat pf0001 = __p(0x1000000000000000);
const PFloat pf0010 = __p(0x2000000000000000);
const PFloat pf0011 = __p(0x3000000000000000);
const PFloat pf0100 = __p(0x4000000000000000);
const PFloat pf0101 = __p(0x5000000000000000);
const PFloat pf0110 = __p(0x6000000000000000);
const PFloat pf0111 = __p(0x7000000000000000);
const PFloat pf1000 = __p(0x8000000000000000);
const PFloat pf1001 = __p(0x9000000000000000);
const PFloat pf1010 = __p(0xA000000000000000);
const PFloat pf1011 = __p(0xB000000000000000);
const PFloat pf1100 = __p(0xC000000000000000);
const PFloat pf1101 = __p(0xD000000000000000);
const PFloat pf1110 = __p(0xE000000000000000);
const PFloat pf1111 = __p(0xF000000000000000);

void set_PFloat4(){
  PENV = &PFloat4_ENV;
}
