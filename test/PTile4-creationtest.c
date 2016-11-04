//PTile4-creationtest.c - tests if we can create the PTile4 numerical system
//using simple parameters.
#include "PTile4-test.h"
#include "../include/penv.h"
#include "../src/PTile4.h"
#include <stdio.h>

static void check_array(const unsigned long long *a1, const unsigned long long *a2, int size){
  int index;
  for (index = 0; index < size; index++){
    printf("testing: %i \n", index);
    printf("%016llx ==? %016llx\n", a1[index], a2[index]);
    assert(a1[index] == a2[index]);
  }
}

void PTile4_creationtest(){
  PEnv *PTile4_test;
  double l4[] = {2.0};
  PTile4_test = create_PTile_environment(l4, 1, 1, 4.0);

  assert(PTile4_test->latticebits == 1);
  assert(PTile4_test->epochbits == 1);
  assert(PTile4_test->increment == 0x1000000000000000);
  check_array(PTile4_test->tables[__ADD_TABLE], __PTile4_addition_table, 4);
  check_array(PTile4_test->tables[__ADD_INVERTED_TABLE], __PTile4_inverted_addition_table, 4);
  check_array(PTile4_test->tables[__ADD_CROSSED_TABLE], __PTile4_crossed_addition_table, 4);
  check_array(PTile4_test->tables[__SUB_TABLE], __PTile4_subtraction_table, 4);
  check_array(PTile4_test->tables[__SUB_EPOCH_TABLE], __PTile4_subtraction_epoch_table, 4);
  check_array(PTile4_test->tables[__SUB_INVERTED_TABLE], __PTile4_inverted_subtraction_table, 4);
  check_array(PTile4_test->tables[__SUB_INVERTED_EPOCH_TABLE], __PTile4_inverted_subtraction_epoch_table, 4);
  check_array(PTile4_test->tables[__SUB_CROSSED_TABLE], __PTile4_crossed_subtraction_table, 4);
  check_array(PTile4_test->tables[__SUB_CROSSED_EPOCH_TABLE], __PTile4_crossed_subtraction_epoch_table, 4);
  check_array(PTile4_test->tables[__MUL_TABLE], __PTile4_multiplication_table, 1);
  check_array(PTile4_test->tables[__DIV_TABLE], __PTile4_division_table, 1);
  check_array(PTile4_test->tables[__INV_TABLE], __PTile4_inversion_table, 1);
}
