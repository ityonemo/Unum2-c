#include "PTile4.h"

const unsigned long long __PTile4_addition_table[4] = {
  0x0000000000000002, 0x0000000000000003,
  0x0000000000000003, 0x0000000000000000};

const unsigned long long __PTile4_inverted_addition_table[4] = {
  0x0000000000000002, 0x0000000000000003,
  0x0000000000000003, 0x0000000000000000};

const unsigned long long __PTile4_crossed_addition_table[4] = {
  0x0000000000000002, 0x0000000000000001,
  0x0000000000000003, 0x0000000000000003};

const unsigned long long __PTile4_subtraction_table[4] = {
  0xffffffffffffffff, 0xffffffffffffffff,
  0x0000000000000000, 0xffffffffffffffff};

const unsigned long long __PTile4_subtraction_epoch_table[4] = {
  0xffffffffffffffff, 0xffffffffffffffff,
  0x0000000000000000, 0xffffffffffffffff};

const unsigned long long __PTile4_inverted_subtraction_table[4] = {
  0xffffffffffffffff, 0x0000000000000002,
  0xffffffffffffffff, 0xffffffffffffffff};

const unsigned long long __PTile4_inverted_subtraction_epoch_table[4] = {
  0xffffffffffffffff, 0x0000000000000000,
  0xffffffffffffffff, 0xffffffffffffffff};

const unsigned long long __PTile4_crossed_subtraction_table[4] = {
  0xffffffffffffffff, 0x0000000000000002,
  0x0000000000000000, 0x0000000000000001};

const unsigned long long __PTile4_crossed_subtraction_epoch_table[4] = {
  0xffffffffffffffff, 0x0000000000000001,
  0x0000000000000000, 0x0000000000000000};

const unsigned long long __PTile4_multiplication_table[1] = {
  0x0000000000000000};

const unsigned long long __PTile4_division_table[1] = {
  0x0000000000000000};

const unsigned long long __PTile4_inverted_division_table[1] = {
  0x0000000000000000};

const unsigned long long __PTile4_inversion_table[1] = {0x0000000000000002};

const PEnv PTile4_ENV = {
  2,
  1,
  0x1000000000000000,
  { __PTile4_addition_table,
    __PTile4_inverted_addition_table,
    __PTile4_crossed_addition_table,
    __PTile4_subtraction_table,
    __PTile4_subtraction_epoch_table,
    __PTile4_inverted_subtraction_table,
    __PTile4_inverted_subtraction_epoch_table,
    __PTile4_crossed_subtraction_table,
    __PTile4_crossed_subtraction_epoch_table,
    __PTile4_multiplication_table,
    __PTile4_division_table,
    __PTile4_inverted_division_table,
    __PTile4_inversion_table }
};
