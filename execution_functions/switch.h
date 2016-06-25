#ifndef _SWITCH_H
#define _SWITCH_H

#include "../structures.h"

struct Value switch_case(struct Tree * ast, struct Tree_map * defined, struct Map * let_map);

#endif
