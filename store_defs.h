#ifndef _STORE_DEFS_H
#define _STORE_DEFS_H

#include "structures.h"

int store_defs(struct Tree * ast, struct Tree_map * defined);
void copy_defs(struct Tree_map * imported, struct Tree_map * defined);

#endif
