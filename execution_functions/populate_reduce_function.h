#ifndef _POPULATE_REDUCE_FUNCTION_H
#define _POPULATE_REDUCE_FUNCTION_H

#include "../structures/structures.h"

void populate_reduce_function(
  struct Value * item_key,
  struct Value * index_key,
  struct Tree * ast,
  struct Value * item,
  struct Value * index);

#endif
