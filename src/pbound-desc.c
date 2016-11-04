#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

void describe(PBound *value){
  switch (value->state){
    case EMPTYSET:
      printf("empty set\n");
    break;
    case ALLREALS:
      printf("all real numbers\n");
    break;
    case SINGLETON:
      printf("singleton: 0x%016llx\n", value->lower);
    break;
    case STDBOUND:
      printf("bound: 0x%016llx, 0x%016llx\n", value->lower, value->upper);
    break;
    default:
      printf("unrecognized PBound state: %i\n", value->state);
  }
}
