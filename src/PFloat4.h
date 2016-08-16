#ifndef __PFLOAT4_H
#define __PFLOAT4_H

#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/pfloat.h"

extern const unsigned long long __PFloat4_addition_table[4];
extern const unsigned long long __PFloat4_inverted_addition_table[4];
extern const unsigned long long __PFloat4_crossed_addition_table[4];
extern const unsigned long long __PFloat4_subtraction_table[4];
extern const unsigned long long __PFloat4_subtraction_epoch_table[4];
extern const unsigned long long __PFloat4_inverted_subtraction_table[4];
extern const unsigned long long __PFloat4_inverted_subtraction_epoch_table[4];
extern const unsigned long long __PFloat4_crossed_subtraction_table[4];
extern const unsigned long long __PFloat4_crossed_subtraction_epoch_table[4];
extern const unsigned long long __PFloat4_multiplication_table[1];
extern const unsigned long long __PFloat4_division_table[1];
extern const unsigned long long __PFloat4_inversion_table[1];

//declare the existence of PFloat4
extern const PEnv PFloat4_ENV;

#endif
