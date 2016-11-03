
#include "PTile4-test.h"

extern PEnv PTile4_ENV;

const PTile pf0000 = __p(0x0000000000000000);
const PTile pf0001 = __p(0x1000000000000000);
const PTile pf0010 = __p(0x2000000000000000);
const PTile pf0011 = __p(0x3000000000000000);
const PTile pf0100 = __p(0x4000000000000000);
const PTile pf0101 = __p(0x5000000000000000);
const PTile pf0110 = __p(0x6000000000000000);
const PTile pf0111 = __p(0x7000000000000000);
const PTile pf1000 = __p(0x8000000000000000);
const PTile pf1001 = __p(0x9000000000000000);
const PTile pf1010 = __p(0xA000000000000000);
const PTile pf1011 = __p(0xB000000000000000);
const PTile pf1100 = __p(0xC000000000000000);
const PTile pf1101 = __p(0xD000000000000000);
const PTile pf1110 = __p(0xE000000000000000);
const PTile pf1111 = __p(0xF000000000000000);

const PBound pb0000 = {__p(0x0000000000000000), __zero, SINGLETON};
const PBound pb0001 = {__p(0x1000000000000000), __zero, SINGLETON};
const PBound pb0010 = {__p(0x2000000000000000), __zero, SINGLETON};
const PBound pb0011 = {__p(0x3000000000000000), __zero, SINGLETON};
const PBound pb0100 = {__p(0x4000000000000000), __zero, SINGLETON};
const PBound pb0101 = {__p(0x5000000000000000), __zero, SINGLETON};
const PBound pb0110 = {__p(0x6000000000000000), __zero, SINGLETON};
const PBound pb0111 = {__p(0x7000000000000000), __zero, SINGLETON};
const PBound pb1000 = {__p(0x8000000000000000), __zero, SINGLETON};
const PBound pb1001 = {__p(0x9000000000000000), __zero, SINGLETON};
const PBound pb1010 = {__p(0xA000000000000000), __zero, SINGLETON};
const PBound pb1011 = {__p(0xB000000000000000), __zero, SINGLETON};
const PBound pb1100 = {__p(0xC000000000000000), __zero, SINGLETON};
const PBound pb1101 = {__p(0xD000000000000000), __zero, SINGLETON};
const PBound pb1110 = {__p(0xE000000000000000), __zero, SINGLETON};
const PBound pb1111 = {__p(0xF000000000000000), __zero, SINGLETON};

void set_PTile4(){
  PENV = (PEnv *)(&PTile4_ENV);
}
