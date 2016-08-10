#include <stdio.h>
#include "PFloat4-test.h"

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
