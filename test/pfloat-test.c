#include <stdio.h>
#include "PFloat4-test.h"

int main(){
  set_PFloat4();
  PFloat4_paritytests();
  PFloat4_inversetests();
  printf("OK\n");
  return 0;
}
