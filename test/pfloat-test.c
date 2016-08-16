#include <stdio.h>
#include "PFloat4-test.h"
#include "../src/PFloat4.h"

int main(){
  set_PFloat4();
  PFloat4_paritytests();
  PFloat4_inversetests();
  PFloat4_itertests();
  PFloat4_synthtests();
  PFloat4_multest();
  PFloat4_addtest();
  PFloat4_bound_math_tests();
  PFloat4_creationtest();
  printf("OK\n");
  return 0;
}
