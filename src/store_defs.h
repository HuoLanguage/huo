#ifndef _STORE_DEFS_H
#define _STORE_DEFS_H

#include "structures/structures.h"

size_t store_defs(struct Tree * ast, hash_table * defined);
struct Tree * read_import(struct String file_name);

#endif
