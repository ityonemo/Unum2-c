#include "../include/pbound.h"
#include "../include/pfloat.h"

void describe(PBound *value){
  switch (value->state){
    EMPTYSET:
      println("empty set");
    break;
    ALLREALS:
      println("all real numbers");
    break;
    SINGLETON:
      println("singleton: %i", bits(value->lower));
    break;
    STDBOUND:
      println("bound: %i, %i", bits(value->lower), bits(value->upper));
    break;
  }
}

int bits(PFloat v){
  return 0;
}
