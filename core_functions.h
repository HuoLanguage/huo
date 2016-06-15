#ifndef _CORE_FUNCTIONS_H
#define _CORE_FUNCTIONS_H

#include "structures.h"

void print(struct Value a);
struct Value add(struct Value a, struct Value b);
struct Value mul(struct Value a, struct Value b);
struct Value sub(struct Value a, struct Value b);
struct Value divide(struct Value a, struct Value b);
struct Value concat(struct Value a, struct Value b);
struct Value not(struct Value a, struct Value b);
struct Value equals(struct Value a, struct Value b);
struct Value length(struct Value a);
struct Value array_index(struct Value a, struct Value arr);
struct Value array_push(struct Value a, struct Value arr);
struct Value array_add(struct Value a, struct Value b);
struct Value array_set(struct Value index, struct Value item, struct Value array);

#endif
