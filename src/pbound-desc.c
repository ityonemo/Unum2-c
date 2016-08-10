#include "../include/pbound.h"
#include "../include/pfloat.h"
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
      printf("singleton: %i\n", bits(value->lower));
    break;
    case STDBOUND:
      printf("bound: %i, %i\n", bits(value->lower), bits(value->upper));
    break;
    default:
      printf("unrecognized PBound state: %i\n", value->state);
  }
}

int bits(PFloat v){
  return 0;
}
