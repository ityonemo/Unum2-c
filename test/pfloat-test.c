#include <stdio.h>
#include "PFloat4-test.h"
#include "../src/PFloat4.h"


extern const unsigned long long __PFloat4_addition_table[];
extern const unsigned long long __PFloat4_inverted_addition_table[];
extern const unsigned long long __PFloat4_crossed_addition_table[];
extern const unsigned long long __PFloat4_subtraction_table[];

int main(){
  set_PFloat4();
  PFloat4_paritytests();
  PFloat4_inversetests();
  PFloat4_itertests();
  PFloat4_synthtests();
  PFloat4_multest();
  printf("OK\n");
  return 0;
}
