#ifndef _STORE_DEFS_H
#define _STORE_DEFS_H

#include "structures.h"

int store_defs(struct Tree * ast, struct Tree_map * defined);
struct Tree * read_import(struct String file_name);

#endif
