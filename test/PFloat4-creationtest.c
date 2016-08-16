//PFloat4-creationtest.c - tests if we can create the PFloat4 numerical system
//using simple parameters.
#include "PFloat4-test.h"
#include "../include/penv.h"
#include "../src/PFloat4.h"

static void check_array(const unsigned long long *a1, const unsigned long long *a2, int size){
  int index;
  for (index = 0; index < size; index++){
    printf("testing: %i \n", index);
    printf("%016llx ==? %016llx\n", a1[index], a2[index]);
    assert(a1[index] == a2[index]);
  }
}

void PFloat4_creationtest(){
  PEnv *pfloat4_test;
  double l4[] = {2.0};
  pfloat4_test = create_pfloat_environment(l4, 1, 1, 4.0);

  assert(pfloat4_test->latticebits == 1);
  assert(pfloat4_test->epochbits == 1);
  assert(pfloat4_test->increment == 0x1000000000000000);
  check_array(pfloat4_test->tables[__ADD_TABLE], __PFloat4_addition_table, 4);
  check_array(pfloat4_test->tables[__ADD_INVERTED_TABLE], __PFloat4_inverted_addition_table, 4);
  check_array(pfloat4_test->tables[__ADD_CROSSED_TABLE], __PFloat4_crossed_addition_table, 4);
  check_array(pfloat4_test->tables[__SUB_TABLE], __PFloat4_subtraction_table, 4);
  check_array(pfloat4_test->tables[__SUB_EPOCH_TABLE], __PFloat4_subtraction_epoch_table, 4);
  check_array(pfloat4_test->tables[__SUB_INVERTED_TABLE], __PFloat4_inverted_subtraction_table, 4);
  check_array(pfloat4_test->tables[__SUB_INVERTED_EPOCH_TABLE], __PFloat4_inverted_subtraction_epoch_table, 4);
  check_array(pfloat4_test->tables[__SUB_CROSSED_TABLE], __PFloat4_crossed_subtraction_table, 4);
  check_array(pfloat4_test->tables[__SUB_CROSSED_EPOCH_TABLE], __PFloat4_crossed_subtraction_epoch_table, 4);
  check_array(pfloat4_test->tables[__MUL_TABLE], __PFloat4_multiplication_table, 1);
  check_array(pfloat4_test->tables[__DIV_TABLE], __PFloat4_division_table, 1);
  check_array(pfloat4_test->tables[__INV_TABLE], __PFloat4_inversion_table, 1);
}
