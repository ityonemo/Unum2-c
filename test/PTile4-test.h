#ifndef __PTile4_TEST_H
#define __PTile4_TEST_H

#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <assert.h>

const PTile pf0000;
const PTile pf0001;
const PTile pf0010;
const PTile pf0011;
const PTile pf0100;
const PTile pf0101;
const PTile pf0110;
const PTile pf0111;
const PTile pf1000;
const PTile pf1001;
const PTile pf1010;
const PTile pf1011;
const PTile pf1100;
const PTile pf1101;
const PTile pf1110;
const PTile pf1111;

const PBound pb0000;
const PBound pb0001;
const PBound pb0010;
const PBound pb0011;
const PBound pb0100;
const PBound pb0101;
const PBound pb0110;
const PBound pb0111;
const PBound pb1000;
const PBound pb1001;
const PBound pb1010;
const PBound pb1011;
const PBound pb1100;
const PBound pb1101;
const PBound pb1110;
const PBound pb1111;

void set_PTile4();
void PTile4_paritytests();
void PTile4_inversetests();
void PTile4_itertests();
void PTile4_synthtests();
void PTile4_multest();
void PTile4_addtest();

#endif
