#ifndef __PFLOAT4_TEST_H
#define __PFLOAT4_TEST_H

#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"
#include <assert.h>

extern const PFloat pf0000;
extern const PFloat pf0001;
extern const PFloat pf0010;
extern const PFloat pf0011;
extern const PFloat pf0100;
extern const PFloat pf0101;
extern const PFloat pf0110;
extern const PFloat pf0111;
extern const PFloat pf1000;
extern const PFloat pf1001;
extern const PFloat pf1010;
extern const PFloat pf1011;
extern const PFloat pf1100;
extern const PFloat pf1101;
extern const PFloat pf1110;
extern const PFloat pf1111;

extern const PBound pb0000;
extern const PBound pb0001;
extern const PBound pb0010;
extern const PBound pb0011;
extern const PBound pb0100;
extern const PBound pb0101;
extern const PBound pb0110;
extern const PBound pb0111;
extern const PBound pb1000;
extern const PBound pb1001;
extern const PBound pb1010;
extern const PBound pb1011;
extern const PBound pb1100;
extern const PBound pb1101;
extern const PBound pb1110;
extern const PBound pb1111;

void set_PFloat4();
void PFloat4_paritytests();
void PFloat4_inversetests();
void PFloat4_itertests();
void PFloat4_synthtests();
void PFloat4_multest();

#endif
