#include "../include/pbound.h"
#include "../include/ptile.h"

void additiveinverse(PBound *v){
  PTile temp;
  switch (v->state){
    case SINGLETON:
      v->lower = tile_additiveinverse(v->lower);
    break;
    case STDBOUND:
      temp = tile_additiveinverse(v->lower);
      v->lower = tile_additiveinverse(v->upper);
      v->upper = temp;
    break;
    default:
    break;
  }
}

void multiplicativeinverse(PBound *v){
  PTile temp;
  switch (v->state){
    case SINGLETON:
      v->lower = tile_multiplicativeinverse(v->lower);
    break;
    case STDBOUND:
      temp = tile_multiplicativeinverse(v->lower);
      v->lower = tile_multiplicativeinverse(v->upper);
      v->upper = temp;
    break;
    default:
    break;
  }
};
