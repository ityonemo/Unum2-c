#ifndef __PFLOAT4_TEST_H
#define __PFLOAT4_TEST_H

#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"
#include <assert.h>

const PFloat pf0000;
const PFloat pf0001;
const PFloat pf0010;
const PFloat pf0011;
const PFloat pf0100;
const PFloat pf0101;
const PFloat pf0110;
const PFloat pf0111;
const PFloat pf1000;
const PFloat pf1001;
const PFloat pf1010;
const PFloat pf1011;
const PFloat pf1100;
const PFloat pf1101;
const PFloat pf1110;
const PFloat pf1111;

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

void set_PFloat4();
void PFloat4_paritytests();
void PFloat4_inversetests();
void PFloat4_itertests();
void PFloat4_synthtests();
void PFloat4_multest();
void PFloat4_addtest();

#endif
