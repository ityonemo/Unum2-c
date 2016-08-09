#include "../include/pbound.h"
#include "../include/pfloat.h"

/////////////////////
// PBound comparison functions.  Note that some of these are a bit unusual.

//pbound equality
bool eq(PBound *lhs, PBound *rhs){
  switch(lhs->state){
    case EMPTYSET:
      return (rhs->state == EMPTYSET);
    case SINGLETON:
      return (rhs->state == SINGLETON) && (rhs->lower == lhs->lower);
    case STDBOUND:
      return (rhs->state == STDBOUND) && (rhs->lower == lhs->lower) && (rhs->upper == lhs->upper);
    case ALLREALS:
      return (rhs->state == ALLREALS);
  }
};

extern bool __roundsinf_checked(PBound *);

//////////////////////////////////////////////
//TODO:  FINISH THESE PROCS
bool lt(PBound *lhs, PBound *rhs);  //less than
bool gt(PBound *lhs, PBound *rhs);  //greater than


bool in(PBound *lhs, PBound *rhs){
  if (rhs->state == ALLREALS) {return true;}
  if (rhs->state == EMPTYSET) {return false;}

  switch(lhs->state){
    case EMPTYSET:
      return true;
    case SINGLETON:
      if (rhs->state == SINGLETON) {return eq(lhs, rhs);};
      if (__roundsinf_checked(rhs)){
        return __s(rhs->lower) <= __s(lhs->lower) ||   //NB that this is a logical OR.
               __s(rhs->upper) >= __s(lhs->lower);
      }
      else {
        return ((rhs->lower == __inf) || __s(rhs->lower) <= __s(lhs->lower)) &&
               ((rhs->upper == __inf) || __s(rhs->upper) >= __s(lhs->lower));
      }
    case STDBOUND:
      if (rhs->state == SINGLETON) {return false;};  //this cannot be true.
      if (__roundsinf_checked(rhs)){
        if (!__roundsinf_checked(lhs)) {return false;};  //rounding infinity must match
        return (__s(rhs->lower) <= __s(lhs->lower)) &&
               (__s(rhs->upper) >= __s(rhs->upper));
      }
      else {
        if (__roundsinf_checked(lhs)) {return false;};  //rounding infinity  must match.
        return ((rhs->lower == __inf) || __s(rhs->lower) <= __s(lhs->lower)) &&
               ((rhs->upper == __inf) || __s(rhs->upper) >= __s(lhs->upper));
      }
    case ALLREALS:
      return false;
  }
}

bool ol(PBound *lhs, PBound *rhs);  //lhs and rhs overlap
