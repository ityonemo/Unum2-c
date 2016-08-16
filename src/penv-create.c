//penv-create.c.  Creates on the heap a pfloat environment, with the specified parameters.

#include "../include/penv.h"
#include <stdlib.h>

typedef struct {
  int latticebits;
  int epochbits;
  unsigned long long increment;
  unsigned long long *tables[__INV_TABLE + 1];  //the list of lookup tables
} __penv_mutable;

int addsub_index(long long lhs_lattice, long long rhs_lattice);

int lattice_length(int latticebits){ return (1 << latticebits) - 1;}

unsigned long long search_lattice(double *lattice, int lattice_length, double value){
  if (value == 1.0) {return 0x0000000000000000;}

  int idx;
  for (idx = 0; idx < lattice_length; idx++){
    if (value == lattice[idx]) {
      return (idx + 1) * 2;
    } else if (value < lattice[idx]) {
      return (idx * 2) + 1;
    }
  }
  return (lattice_length * 2) + 1;
}

static void create_addition_table(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *table = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));

  int idx1, idx2;
  for (idx1 = 0; idx1 <= latticelength; idx1++)
    for (idx2 = 0; idx2 <= latticelength; idx2++){
      //assign the test value.

      test_value = ((idx1 == 0) ? 1 : lattice[idx1 - 1]) + ((idx2 == 0) ? 1 : lattice[idx2 - 1]);

      //check if the true value exceeds the pivot value.
      if (test_value >= pivot) {test_value /= pivot;}
      //search for the value we want.
      table[addsub_index(idx1 + 1, idx2 + 1)] = search_lattice(lattice, latticelength, test_value);
    }
  //assign the appropriate tables slot to the table pointer.
  tables[__ADD_TABLE] = table;
}

////////////////////////////////////////////////////////////////////////////////

static void create_inverted_addition_table(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *table = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));

  int idx1, idx2;
  for (idx1 = 0; idx1 <= latticelength; idx1++)
    for (idx2 = 0; idx2 <= latticelength; idx2++){
      //assign the test value.
      test_value = 1/(((idx1 == 0) ? 1 : 1/lattice[idx1 - 1]) + ((idx2 == 0) ? 1 : 1/lattice[idx2 - 1]));

      //check if the true value exceeds the pivot value.
      if (test_value < 1) {test_value *= pivot;}
      //search for the value we want.
      table[addsub_index(idx1 + 1, idx2 + 1)] = search_lattice(lattice, latticelength, test_value);
    }
  //assign the appropriate tables slot to the table pointer.
  tables[__ADD_INVERTED_TABLE] = table;
}

////////////////////////////////////////////////////////////////////////////////

static void create_crossed_addition_table(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *table = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));

  int idx1, idx2;
  for (idx1 = 0; idx1 <= latticelength; idx1++)
    for (idx2 = 0; idx2 <= latticelength; idx2++){
      //assign the test value.
      test_value = (((idx1 == 0) ? 1 : lattice[idx1 - 1]) + ((idx2 == 0) ? 1 : 1/lattice[idx2 - 1]));

      //check if the true value exceeds the pivot value.
      if (test_value >= pivot) {test_value /= pivot;}

      //search for the value we want.
      table[addsub_index(idx1 + 1, idx2 + 1)] = search_lattice(lattice, latticelength, test_value);
    }

  //assign the appropriate tables slot to the table pointer.
  tables[__ADD_CROSSED_TABLE] = table;
}

////////////////////////////////////////////////////////////////////////////////

static unsigned long long search_epochs(double *true_value, double pivot_value){
  unsigned long long epoch_delta = 0;
  while (*true_value < 1.0){
    (*true_value) *= pivot_value;
    epoch_delta += 1;
  }
  return epoch_delta;
}

////////////////////////////////////////////////////////////////////////////////

static void create_subtraction_tables(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *vtable = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));
  unsigned long long *etable = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));

  int idx1, idx2;
  for (idx1 = 0; idx1 <= latticelength; idx1++)
    for (idx2 = 0; idx2 <= latticelength; idx2++){
      if (idx1 > idx2){
        //assign the test value.
        test_value = (((idx1 == 0) ? 1 : lattice[idx1 - 1]) - ((idx2 == 0) ? 1 : lattice[idx2 - 1]));

        //check if the true value exceeds the pivot value.
        unsigned long long epoch_value = search_epochs(&test_value, pivot);

        //search for the value we want.
        vtable[addsub_index(idx1 + 1, idx2 + 1)] = search_lattice(lattice, latticelength, test_value);
        etable[addsub_index(idx1 + 1, idx2 + 1)] = epoch_value;
      } else {
        vtable[addsub_index(idx1 + 1, idx2 + 1)] = 0xFFFFFFFFFFFFFFFF;
        etable[addsub_index(idx1 + 1, idx2 + 1)] = 0xFFFFFFFFFFFFFFFF;
      }
    }

  //assign the appropriate tables slot to the table pointer.
  tables[__SUB_TABLE] = vtable;
  tables[__SUB_EPOCH_TABLE] = etable;
}

////////////////////////////////////////////////////////////////////////////////

static void create_inverted_subtraction_tables(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *vtable = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));
  unsigned long long *etable = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));

  int idx1, idx2;
  for (idx1 = 0; idx1 <= latticelength; idx1++)
    for (idx2 = 0; idx2 <= latticelength; idx2++){
      if (idx1 < idx2){
        //assign the test value.
        test_value = 1/(((idx1 == 0) ? 1 : 1/lattice[idx1 - 1]) - ((idx2 == 0) ? 1 : 1/lattice[idx2 - 1]));

        //check if the true value exceeds the pivot value.
        unsigned long long epoch_value = search_epochs(&test_value, pivot);

        //search for the value we want.
        vtable[addsub_index(idx1 + 1, idx2 + 1)] = search_lattice(lattice, latticelength, test_value);
        etable[addsub_index(idx1 + 1, idx2 + 1)] = epoch_value;
      } else {
        vtable[addsub_index(idx1 + 1, idx2 + 1)] = 0xFFFFFFFFFFFFFFFF;
        etable[addsub_index(idx1 + 1, idx2 + 1)] = 0xFFFFFFFFFFFFFFFF;
      }
    }

  //assign the appropriate tables slot to the table pointer.
  tables[__SUB_INVERTED_TABLE] = vtable;
  tables[__SUB_INVERTED_EPOCH_TABLE] = etable;
}

////////////////////////////////////////////////////////////////////////////////

static void create_crossed_subtraction_tables(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *vtable = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));
  unsigned long long *etable = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength + 1) * (latticelength + 1));

  int idx1, idx2;
  for (idx1 = 0; idx1 <= latticelength; idx1++)
    for (idx2 = 0; idx2 <= latticelength; idx2++){
      if ((idx1 == 0) && (idx2 == 0)){
        vtable[addsub_index(idx1 + 1, idx2 + 1)] = 0xFFFFFFFFFFFFFFFF;
        etable[addsub_index(idx1 + 1, idx2 + 1)] = 0xFFFFFFFFFFFFFFFF;
      } else {
        //assign the test value.
        test_value = ((idx1 == 0) ? 1 : lattice[idx1 - 1]) - ((idx2 == 0) ? 1 : 1/lattice[idx2 - 1]);

        //check if the true value exceeds the pivot value.
        unsigned long long epoch_value = search_epochs(&test_value, pivot);

        //search for the value we want.
        vtable[addsub_index(idx1 + 1, idx2 + 1)] = search_lattice(lattice, latticelength, test_value);
        etable[addsub_index(idx1 + 1, idx2 + 1)] = epoch_value;
      }
    }

  //assign the appropriate tables slot to the table pointer.
  tables[__SUB_CROSSED_TABLE] = vtable;
  tables[__SUB_CROSSED_EPOCH_TABLE] = etable;
}

////////////////////////////////////////////////////////////////////////////////

static void create_multiplication_table(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *table = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength) * (latticelength));

  int idx1, idx2;
  for (idx1 = 1; idx1 <= latticelength; idx1++)
    for (idx2 = 1; idx2 <= latticelength; idx2++){
      //assign the test value.

      test_value = (lattice[idx1 - 1]) * (lattice[idx2 - 1]);

      //check if the true value exceeds the pivot value.
      if (test_value >= pivot) {test_value /= pivot;}
      //search for the value we want.
      table[muldiv_index(idx1 + 1, idx2 + 1)] = search_lattice(lattice, latticelength, test_value);
    }
  //assign the appropriate tables slot to the table pointer.
  tables[__MUL_TABLE] = table;
}

////////////////////////////////////////////////////////////////////////////////

static void create_division_table(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *table = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength) * (latticelength));

  int idx1, idx2;
  for (idx1 = 1; idx1 <= latticelength; idx1++)
    for (idx2 = 1; idx2 <= latticelength; idx2++){
      //assign the test value.

      test_value = (lattice[idx1 - 1]) / (lattice[idx2 - 1]);

      //check if the true value exceeds the pivot value.
      if (test_value < 1) {test_value *= pivot;}
      //search for the value we want.
      table[muldiv_index(idx1 + 1, idx2 + 1)] = search_lattice(lattice, latticelength, test_value);
    }
  //assign the appropriate tables slot to the table pointer.
  tables[__DIV_TABLE] = table;
}

////////////////////////////////////////////////////////////////////////////////

static void create_inversion_table(unsigned long long **tables, double *lattice, int latticebits, int epochbits, double pivot){
  int latticelength = lattice_length(latticebits);
  double test_value;

  //allocate the memory for this thing.
  unsigned long long *table = (unsigned long long*) malloc(sizeof(unsigned long long) * (latticelength));

  int idx;
  for (idx = 1; idx <= latticelength; idx++){
      //assign the test value.

      test_value = pivot / lattice[idx - 1];

      //search for the value we want.
      table[idx - 1] = search_lattice(lattice, latticelength, test_value);
    }
  //assign the appropriate tables slot to the table pointer.
  tables[__INV_TABLE] = table;
}

////////////////////////////////////////////////////////////////////////////////

PEnv *create_pfloat_environment(double *lattice, int latticebits, int epochbits, double pivot){
  __penv_mutable *environment = malloc(sizeof(__penv_mutable));
  environment->latticebits = latticebits;
  environment->epochbits = epochbits;
  environment->increment = (0x0000000000000001LL << (62 - latticebits - epochbits));

  create_addition_table(environment->tables, lattice, latticebits, epochbits, pivot);
  create_inverted_addition_table(environment->tables, lattice, latticebits, epochbits, pivot);
  create_crossed_addition_table(environment->tables, lattice, latticebits, epochbits, pivot);

  create_subtraction_tables(environment->tables, lattice, latticebits, epochbits, pivot);
  create_inverted_subtraction_tables(environment->tables, lattice, latticebits, epochbits, pivot);
  create_crossed_subtraction_tables(environment->tables, lattice, latticebits, epochbits, pivot);

  create_multiplication_table(environment->tables, lattice, latticebits, epochbits, pivot);
  create_division_table(environment->tables, lattice, latticebits, epochbits, pivot);
  create_inversion_table(environment->tables, lattice, latticebits, epochbits, pivot);

  return (PEnv *)environment;
}

void delete_pfloat_environment(PEnv *environment){
  int index;
  for (index = 0; index < __INV_TABLE; index++){
    free((void *)environment->tables[index]);
  }
  free((void *)environment);
}
