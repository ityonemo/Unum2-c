#ifndef __PENV_H
#define __PENV_H

//an eneumeration of the order of tables.
enum{
  _ADD_TABLE,
  _ADD_CROSSED_TABLE,
  _ADD_INVERTED_TABLE,
  _SUB_TABLE,
  _SUB_EPOCH_TABLE,
  _SUB_CROSSED_TABLE,
  _SUB_CROSSED_EPOCH_TABLE,
  _SUB_INVERTED_TABLE,
  _SUB_INVERTED_EPOCH_TABLE,
  _MUL_TABLE,
  _DIV_TABLE,
  _INV_TABLE
};

// The environment struct will be defined here.  We should use the PEnv type, everywhere.
typedef struct {
  const int latticebits;
  const int epochbits;
  const unsigned long long increment;
  const unsigned long long *tables[];  //the list of lookup tables
} PEnv;

////////////////////////////////////////////////////////////////////////////////
// ENVIRONMENT functions

extern PEnv* PENV;

#endif
