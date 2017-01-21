#include "../include/penv.h"
#include "../include/pbound.h"
#include "../include/ptile.h"
#include <stdio.h>

void sub(PBound *dest, const PBound *lhs, const PBound *rhs){
  //allocate on the stack the rhs value, invert it, then add.
  PBound r_inv = __EMPTYBOUND;
  pcopy(&r_inv, rhs);
  additiveinverse(&r_inv);
  sub(dest, lhs, &r_inv);
}

static PTile exact_arithmetic_subtraction_uninverted(__dc_tile *outer, __dc_tile *inner){
  //ascertain the table that we'll need to look up.
  int table = outer->epoch - inner->epoch;
  int lookup_index = table_addsub_index(table, outer->lattice, inner->lattice);

  if (table < PENV->table_counts[__SUB_TABLE]){
    outer->lattice = (PENV->tables[__SUB_TABLE])[lookup_index];
    outer->epoch = outer->epoch - (PENV->tables[__SUB_EPOCH_TABLE])[lookup_index];
  } else {
    return (outer->negative ? next(tile_synth(outer)) : prev(tile_synth(outer)));
  }

  if (outer->epoch < 0){
    outer->inverted = true;
    outer->epoch = 0;
    outer->lattice = (PENV->tables[__INVERTED_SUB_TABLE])[lookup_index];
  }

  return tile_synth(outer);
}

static PTile exact_arithmetic_subtraction_inverted(__dc_tile *outer, __dc_tile *inner){
  //ascertain the table that we'll need to look up.
  int table = inner->epoch - outer->epoch;
  int index = table_addsub_index(table, outer->lattice, inner->lattice);

  if (table < PENV->table_counts[__SUB_INVERTED_TABLE]) {
    outer->lattice = (PENV->tables[__SUB_INVERTED_TABLE])[index];
    outer->epoch = outer->epoch + (PENV->tables[__SUB_INVERTED_EPOCH_TABLE])[index];
  } else {
    return (outer->negative ? next(tile_synth(outer)) : prev(tile_synth(outer)));
  }

  return tile_synth(outer);
}

static PTile exact_arithmetic_subtraction_crossed(__dc_tile *outer, __dc_tile *inner){
  //ascertain the table that we'll need to look up.
  int table = outer->epoch + inner->epoch;
  int index = table_addsub_index(table, outer->lattice, inner->lattice);

  if (table < PENV->table_counts[__SUB_CROSSED_TABLE]){
    outer->lattice = (PENV->tables[__SUB_CROSSED_TABLE])[index];
    outer->epoch = outer->epoch - (PENV->tables[__SUB_CROSSED_EPOCH_TABLE])[index];
  } else {
    return (outer->negative ? next(tile_synth(outer)) : prev(tile_synth(outer)));
  }

  if (outer->epoch < 0){
    outer->inverted = true;
    outer->epoch = 0;
    outer->lattice = (PENV->tables[__INVERTED_SUB_CROSSED_TABLE])[index];
  }

  return tile_synth(outer);
}


PTile dc_arithmetic_subtraction(__dc_tile *outer, __dc_tile *inner){
  if (outer->inverted ^ inner->inverted) {
    return exact_arithmetic_subtraction_crossed(outer, inner);
  } else if (outer->inverted) {
    return exact_arithmetic_subtraction_inverted(outer, inner);
  } else {
    return exact_arithmetic_subtraction_uninverted(outer, inner);
  }
}
