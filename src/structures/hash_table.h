#ifndef _HUO_HASH_TABLE_H
#define _HUO_HASH_TABLE_H
#include <stdbool.h>

extern const void *NO_ELEMENT;

typedef struct hash_table_t hash_table;
typedef struct hash_table_iter_t hash_table_iter;

typedef unsigned long (hash_code_func)(void *);
typedef bool (equals_func)(void *, void *);


hash_table *hash_table_new(hash_code_func *hash_gen, equals_func *equality_test);
void *hash_table_put(hash_table *table, void *key, void *val); // Returns old val, or null
void *hash_table_get(hash_table *table, void *key);
void *hash_table_remove(hash_table *table, void *key);

bool hash_table_contains(hash_table *table, void *key);
unsigned long hash_table_size(hash_table *table);

hash_table_iter *hash_table_iter_new(hash_table *table);
void *hash_table_iter_key(hash_table_iter *iter);
void *hash_table_iter_val(hash_table_iter *iter);
bool hash_table_iter_has_cur(hash_table_iter *iter);
bool hash_table_iter_next(hash_table_iter *iter);

#endif