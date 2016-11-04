#ifndef __PENV_H
#define __PENV_H

//an eneumeration of the order of tables.
enum{
  __ADD_TABLE,
  __ADD_INVERTED_TABLE,
  __ADD_CROSSED_TABLE,
  __SUB_TABLE,
  __SUB_EPOCH_TABLE,
  __SUB_INVERTED_TABLE,
  __SUB_INVERTED_EPOCH_TABLE,
  __SUB_CROSSED_TABLE,
  __SUB_CROSSED_EPOCH_TABLE,
  __MUL_TABLE,
  __DIV_TABLE,
  __DIV_INV_TABLE,
  __INV_TABLE
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

PEnv *create_PTile_environment(double *lattice, int latticebits, int epochbits, double stride);

#endif
