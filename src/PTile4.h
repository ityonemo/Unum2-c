#ifndef __PTile4_H
#define __PTile4_H

#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/PTile.h"

extern const unsigned long long __PTile4_addition_table[4];
extern const unsigned long long __PTile4_inverted_addition_table[4];
extern const unsigned long long __PTile4_crossed_addition_table[4];
extern const unsigned long long __PTile4_subtraction_table[4];
extern const unsigned long long __PTile4_subtraction_epoch_table[4];
extern const unsigned long long __PTile4_inverted_subtraction_table[4];
extern const unsigned long long __PTile4_inverted_subtraction_epoch_table[4];
extern const unsigned long long __PTile4_crossed_subtraction_table[4];
extern const unsigned long long __PTile4_crossed_subtraction_epoch_table[4];
extern const unsigned long long __PTile4_multiplication_table[1];
extern const unsigned long long __PTile4_division_table[1];
extern const unsigned long long __PTile4_inversion_table[1];

//declare the existence of PTile4
extern const PEnv PTile4_ENV;

#endif
