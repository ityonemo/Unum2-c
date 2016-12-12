#include <stdio.h>
#include "PFloat4.h"
#include "PTile4-test.h"

int main(){
  set_PTile4();
  PTile4_paritytests();
  PTile4_inversetests();
  PTile4_itertests();
  PTile4_synthtests();
  PTile4_multest();
  PTile4_addtest();
  PTile4_bound_math_tests();
  printf("OK\n");
  return 0;
}
