#ifndef __PENV_H
#define __PENV_H

//an eneumeration of the order of tables.
enum{
  //the six primary addition and subtraction tables.  Each of these is chunked
  //into subtables which contain data for additions across multiple epochs.
  __ADD_TABLE = 0,
  __ADD_INVERTED_TABLE,
  __ADD_CROSSED_TABLE,
  __SUB_TABLE,
  __SUB_INVERTED_TABLE,
  __SUB_CROSSED_TABLE,

  //inverted tables.  These are the same as the primary tables but contain the
  //lattice result when the result crosses 1.
  __INVERTED_ADD_INVERTED_TABLE,
  __INVERTED_SUB_TABLE,
  __INVERTED_SUB_CROSSED_TABLE,

 //epoch tables.  These contain information about how many epochs have been
 //crossed in subtraction operations.
  __SUB_EPOCH_TABLE,
  __SUB_INVERTED_EPOCH_TABLE,
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
  const int *table_counts;             //list of table counts for the first six tables
  const unsigned long long *tables[];  //the list of lookup tables
} PEnv;

////////////////////////////////////////////////////////////////////////////////
// ENVIRONMENT functions

extern PEnv* PENV;

PEnv *create_PTile_environment(double *lattice, int latticebits, int epochbits, double stride);

#endif
