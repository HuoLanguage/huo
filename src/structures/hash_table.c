#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hash_table.h"
#include "../base_util.h"

void *hash_table_put_int(hash_table *table, void *key, void *val, unsigned long hash_code);
unsigned long get_distance(hash_table *table, unsigned long cur_pos);
void print_table(hash_table *table);
bool hash_table_needs_resize(hash_table *table);
void hash_table_resize(hash_table *table, unsigned long new_size);
bool hash_table_is_sane(hash_table *table);


/*
 * A simple hash table, using robin hood hashing.
 *
 * Basic idea behind "robin hood" hashing: it's like linear probing,
 * but you can "kick out" another slot if you're further from your preferred slot
 * than they are.
 *
 * To use, just implement hash_code_func and equals_func.
 * It must always be the case that equals_func(a,b) implies hash_code_func(a) == hash_code_func(b),
 * but not necessarily the other way around.
 *
 *
 * */

#define HASH_TABLE_MIN_SIZE 16

#define HASH_TABLE_MAX_LOAD_NUM 3
#define HASH_TABLE_MAX_LOAD_DENOM 4

#define HASH_TABLE_MIN_LOAD_NUM 1
#define HASH_TABLE_MIN_LOAD_DENOM 4

#define HASH_TABLE_EXPAND_NUM 3
#define HASH_TABLE_EXPAND_DENOM 2

#define HASH_TABLE_SHRINK_NUM 2
#define HASH_TABLE_SHRINK_DENOM 3

STATIC_ASSERT(HASH_TABLE_MAX_LOAD_NUM * HASH_TABLE_EXPAND_NUM * HASH_TABLE_MIN_LOAD_DENOM  > HASH_TABLE_MIN_LOAD_NUM * HASH_TABLE_MAX_LOAD_DENOM * HASH_TABLE_EXPAND_DENOM, Hash_table_expands_too_much);

STATIC_ASSERT(HASH_TABLE_MIN_LOAD_NUM * HASH_TABLE_SHRINK_NUM * HASH_TABLE_MAX_LOAD_DENOM  < HASH_TABLE_MAX_LOAD_NUM * HASH_TABLE_MIN_LOAD_DENOM * HASH_TABLE_SHRINK_DENOM, Hash_table_shrinks_too_much);

STATIC_ASSERT(HASH_TABLE_EXPAND_NUM > HASH_TABLE_EXPAND_DENOM, Expand_ratio_less_than_or_equal_1);

STATIC_ASSERT(HASH_TABLE_SHRINK_NUM < HASH_TABLE_SHRINK_DENOM, Shrink_ratio_greater_than_or_equal_1);

STATIC_ASSERT(HASH_TABLE_MAX_LOAD_NUM < HASH_TABLE_MAX_LOAD_DENOM, Max_load_greater_than_or_equal_1);

STATIC_ASSERT(HASH_TABLE_MAX_LOAD_NUM * HASH_TABLE_MAX_LOAD_DENOM > 0, Max_load_less_than_or_equal_0);

STATIC_ASSERT(HASH_TABLE_MIN_LOAD_NUM < HASH_TABLE_MIN_LOAD_DENOM, Min_load_greater_than_or_equal_1);

STATIC_ASSERT(HASH_TABLE_MIN_LOAD_NUM * HASH_TABLE_MIN_LOAD_DENOM > 0, Min_load_less_than_or_equal_0);

STATIC_ASSERT(HASH_TABLE_MAX_LOAD_NUM * HASH_TABLE_MIN_LOAD_DENOM > HASH_TABLE_MAX_LOAD_DENOM * HASH_TABLE_MIN_LOAD_NUM, Max_load_less_than_or_equal_min_load);

const char DUMMY = 'd';
#define NO_ELEMENT ((char *) &DUMMY)


struct hash_table_entry_t {
    unsigned long hash_code;
    void *key;
    void *val;
};

struct hash_table_t {
    struct hash_table_entry_t *table;
    unsigned long table_size;
    unsigned long table_alloc_size;
    hash_code_func *hash_gen;
    equals_func *equality_test;
};

struct hash_table_iter_t {
    hash_table *table;
    unsigned long pos;
};

hash_table *hash_table_new(hash_code_func *hash_gen, equals_func *equality_test) {
    hash_table *h = malloc(sizeof(hash_table));
    if (h == NULL) {
        ERROR("Malloc failure");
    }
    h->table = NULL;
    h->table_size = 0;
    h->table_alloc_size = 0;
    h->hash_gen = hash_gen;
    h->equality_test = equality_test;
    hash_table_resize(h, HASH_TABLE_MIN_SIZE);
    assert(hash_table_is_sane(h));
    return h;
}

bool hash_table_slot_is_free(struct hash_table_entry_t *entry) {
    return entry->key == NO_ELEMENT;
}

bool hash_table_is_sane(hash_table *table) {
    //print_table(table);
    assert (table != NULL);
    assert (table->table_size <= table->table_alloc_size);
    assert (table->table_alloc_size >= HASH_TABLE_MIN_SIZE);
    if (table->table_alloc_size > 0)
        assert(table->table != NULL);
    unsigned long num_ele = 0;
    for (unsigned long l = 0; l < table->table_alloc_size; l++) {
        struct hash_table_entry_t *e = &table->table[l];
        if (hash_table_slot_is_free(e))
            continue;
        assert(get_distance(table, l) <= table->table_alloc_size);
        assert(e->hash_code == table->hash_gen(e->key));
        num_ele += 1;
        unsigned long pos = l;
        unsigned long pref_pos = (e->hash_code) % table->table_alloc_size;
        while (true) {
            if (pos == pref_pos)
                break;
            assert (!hash_table_slot_is_free(&table->table[pos]));
            pos = (pos - 1 + table->table_alloc_size) % table->table_alloc_size; // Negative wraparound, otherwise
        }
    }
    assert (num_ele == table->table_size);
    assert (!hash_table_needs_resize(table));
    return true;
}

bool hash_table_too_empty(hash_table *table) {
    unsigned int s = table->table_size;
    unsigned int a = table->table_alloc_size;
    return s > HASH_TABLE_MIN_SIZE && s * HASH_TABLE_MIN_LOAD_DENOM < a * HASH_TABLE_MIN_LOAD_NUM;
}

bool hash_table_too_full(hash_table *table) {
    unsigned int s = table->table_size;
    unsigned int a = table->table_alloc_size;
    return s * HASH_TABLE_MAX_LOAD_DENOM > a * HASH_TABLE_MAX_LOAD_NUM;
}

bool hash_table_needs_resize(hash_table *table) {
    return hash_table_too_empty(table) || hash_table_too_full(table);
}

void hash_table_resize(hash_table *table, unsigned long new_size) {
    //assert(hash_table_is_sane(table));
    // Called by constructor, and as such may have too few elements
    assert (new_size >= table->table_size);
    assert (new_size >= HASH_TABLE_MIN_SIZE);
    unsigned long old_alloc_size = table->table_alloc_size;
    struct hash_table_entry_t *old_table = table->table;

    unsigned long num_ele = table->table_size;
    table->table_size = 0;
    table->table_alloc_size = new_size;
    table->table = ARR_MALLOC(new_size, struct hash_table_entry_t);

    // Set elements to uninitialized
    for (unsigned long i = 0; i < new_size; i++) {
        struct hash_table_entry_t entry = {
            .hash_code = 0,
            .key = NO_ELEMENT,
            .val = NULL
        };
        table->table[i] = entry;
    }

    // Reinsert  old elements where necessary.
    for (unsigned long i = 0; i < old_alloc_size; i++) {
        struct hash_table_entry_t *entry = &old_table[i];
        if (!hash_table_slot_is_free(entry)) {
            hash_table_put_int(table, entry->key, entry->val, entry->hash_code);
        }
    }
    free(old_table);
    assert(hash_table_is_sane(table));
    assert(table->table_size == num_ele);
}

void hash_table_maybe_resize(hash_table *table) {
    unsigned long size = table->table_alloc_size;
    unsigned long new_size;
    if (hash_table_too_full(table)) {
        new_size = size * HASH_TABLE_EXPAND_NUM / HASH_TABLE_EXPAND_DENOM;
        if (new_size <= size)
            new_size = size + 1;
    } else if (hash_table_too_empty(table)) {
        new_size = size * HASH_TABLE_SHRINK_NUM / HASH_TABLE_SHRINK_DENOM;
        if (new_size >= size)
            new_size = size - 1;
    } else {
        return;
    }
    if (new_size < HASH_TABLE_MIN_SIZE)
        new_size = HASH_TABLE_MIN_SIZE;
    hash_table_resize(table, new_size);
    assert (!hash_table_needs_resize(table));
}

unsigned long get_distance(hash_table *table, unsigned long cur_pos) {
    //assert (cur_pos >= 0);
    assert (cur_pos < table->table_alloc_size);
    struct hash_table_entry_t *entry = &table->table[cur_pos];
    assert (!hash_table_slot_is_free(entry));
    unsigned long init_pos = table->table[cur_pos].hash_code % table->table_alloc_size;
    if (init_pos <= cur_pos)
        return cur_pos - init_pos;
    else
        return cur_pos + (table->table_alloc_size - init_pos);
}

void *hash_table_put_int(hash_table *table, void *key, void *val, unsigned long hash_code) {
    unsigned long init_hash_code = hash_code;
    assert(hash_table_is_sane(table));
    while (true) {
        unsigned long probe_current = 0;
        for (unsigned long i = 0; i < table->table_alloc_size; i++) {
            unsigned long table_pos = (init_hash_code + i) % table->table_alloc_size;
            struct hash_table_entry_t *entry = &table->table[table_pos];
            bool was_free = hash_table_slot_is_free(entry);
            if (was_free || (entry->hash_code == hash_code && table->equality_test(entry->key, key))) {
                // Right entry, now kick it out.
                void *old_val = entry->val;
                entry->hash_code = hash_code;
                entry->key = key;
                entry->val = val;
                if (was_free) {
                    table->table_size += 1;
                    hash_table_maybe_resize(table);
                }
                assert(hash_table_is_sane(table));
                return old_val;
            }

            unsigned long probe_distance = get_distance(table, table_pos);

            if (probe_current > probe_distance){
                // swap current entry with entry to insert
                struct hash_table_entry_t temp = *entry;
                entry->hash_code = hash_code;
                entry->key = key;
                entry->val = val;

                hash_code = temp.hash_code;
                key = temp.key;
                val = temp.val;

                probe_current = probe_distance;
            }
            probe_current += 1;
        }
        hash_table_maybe_resize(table);
    }
}

void *hash_table_put(hash_table *table, void *key, void *val) { // Returns old val, or null
    assert(hash_table_is_sane(table));
    unsigned long hash_code = table->hash_gen(key);
    return hash_table_put_int(table, key, val, hash_code);
}

void *hash_table_get(hash_table *table, void *key) {
    assert(hash_table_is_sane(table));
    unsigned long hash_code = table->hash_gen(key);

    for (unsigned long i = 0; i < table->table_alloc_size; i++) {
        unsigned long table_pos = (hash_code + i) % table->table_alloc_size;
        struct hash_table_entry_t *entry = &table->table[table_pos];
        if (hash_table_slot_is_free(entry) || i > get_distance(table, table_pos)) {
            break;
        } else if (entry->hash_code == hash_code && table->equality_test(entry->key, key)) {
            return entry->val;
        }
    }
    return NULL;
}

void *hash_table_remove(hash_table *table, void *key) {
    assert(hash_table_is_sane(table));
    unsigned long hash_code = table->hash_gen(key);
    for (unsigned long i = 0; i < table->table_alloc_size; i++) {
        unsigned long table_pos = (hash_code + i) % table->table_alloc_size;
        struct hash_table_entry_t *entry = &table->table[table_pos];
        if (hash_table_slot_is_free(entry) || i > get_distance(table, table_pos)) {
            assert(hash_table_is_sane(table));
            return NULL;
        } else if (entry->hash_code == hash_code && table->equality_test(entry->key, key)) {
            entry->key = NO_ELEMENT;
            table->table_size -= 1;
            void *old_val = entry->val;
            unsigned long prev_pos = table_pos;
            for (unsigned long j = 1; j < table->table_alloc_size; j++) {
                unsigned long cur_pos = (table_pos + j) % table->table_alloc_size;
                if (hash_table_slot_is_free(&table->table[cur_pos]) ||get_distance(table, cur_pos) == 0) {
                    table->table[prev_pos].key = NO_ELEMENT;
                    break;
                }
                table->table[prev_pos] = table->table[cur_pos];

                prev_pos = cur_pos;
            }
            hash_table_maybe_resize(table);
            assert(hash_table_is_sane(table));
            return old_val;
        }
    }

    assert(hash_table_is_sane(table));
    return NULL;
}

bool hash_table_contains(hash_table *table, void *key) {
    assert(hash_table_is_sane(table));
    unsigned long hash_code = table->hash_gen(key);
    for (unsigned long i = 0; i < table->table_alloc_size; i++) {
        unsigned long table_pos = (hash_code + i) % table->table_alloc_size;
        struct hash_table_entry_t *entry = &table->table[table_pos];
        if (hash_table_slot_is_free(entry)) {
            return false;
        } else if (entry->hash_code == hash_code && table->equality_test(entry->key, key)) {
            return true;
        }
    }
    return false;
}

unsigned long hash_table_size(hash_table *table) {
    assert(hash_table_is_sane(table));
    return table->table_size;
}

hash_table_iter *hash_table_iter_new(hash_table *table) {
    assert(hash_table_is_sane(table));
    hash_table_iter *to_ret = malloc(sizeof(hash_table_iter));
    to_ret->table = table;
    to_ret->pos = 0;
    return to_ret;
}
void advance_if_necessary(hash_table_iter *iter) {
    while (iter->pos < iter->table->table_alloc_size && hash_table_slot_is_free(&iter->table->table[iter->pos]))
        iter->pos += 1;
}
void *hash_table_iter_key(hash_table_iter *iter) {
    advance_if_necessary(iter);
    if (iter->pos >= iter->table->table_alloc_size)
            return NULL;
    assert (!hash_table_slot_is_free(&iter->table->table[iter->pos]));
    return iter->table->table[iter->pos].key;
}
void *hash_table_iter_val(hash_table_iter *iter) {
    advance_if_necessary(iter);
    if (iter->pos >= iter->table->table_alloc_size)
            return NULL;
    assert (!hash_table_slot_is_free(&iter->table->table[iter->pos]));
    return iter->table->table[iter->pos].val;
}
bool hash_table_iter_has_cur(hash_table_iter *iter) {
    advance_if_necessary(iter);
    return (iter->pos < iter->table->table_alloc_size);
}
bool hash_table_iter_next(hash_table_iter *iter) {
    if (iter->pos >= iter->table->table_alloc_size) {
        assert (!hash_table_iter_has_cur(iter));
        return false;
    }
    iter->pos++;
    return true;
}


void print_table(hash_table *table) {
    printf("[");
    for (unsigned long j = 0; j < table->table_alloc_size; j++) {
        if (hash_table_slot_is_free(&table->table[j]))
            printf("null");
        else
            printf("%lu", table->table[j].hash_code % table->table_alloc_size);
        if (j != table->table_alloc_size - 1)
            printf(", ");
    }
    printf("]\n");
}

#if 0

unsigned long hash_code_int(void *long_val) {
    return *((unsigned long *) long_val) / 10;
}

bool equal_int(void *a, void *b) {
    return *((unsigned long *) a) == *((unsigned long *) b);
}

void do_iter_test(hash_table *table, unsigned long *keys,  unsigned long *vals, unsigned long num_elems) {
    unsigned long *so_far_keys = NULL;
    unsigned long *so_far_vals = NULL;
    unsigned long num_steps = 0;
    for (hash_table_iter *iter = hash_table_iter_new(table); hash_table_iter_has_cur(iter); hash_table_iter_next(iter)) {
        so_far_vals = realloc(so_far_vals, (num_steps + 1) * sizeof(unsigned long));
        assert (so_far_vals != NULL);
        so_far_vals[num_steps] = *(unsigned long *) hash_table_iter_val(iter);

        so_far_keys = realloc(so_far_keys, (num_steps + 1) * sizeof(unsigned long));
        assert (so_far_keys != NULL);
        so_far_keys[num_steps] = *(unsigned long *) hash_table_iter_key(iter);

        num_steps += 1;
    }
    printf("%lu %lu\n", num_steps, num_elems);
    assert (num_steps == num_elems);
    for (unsigned long i = 0; i < num_steps; i++) {
        bool so_far_in_kv = false;
        bool kv_in_so_far = false;
        for (unsigned long j = 0; j < num_steps; j++) {
            if (keys[i] == so_far_keys[j] && vals[i] == so_far_vals[j])
                kv_in_so_far = true;
            if (keys[j] == so_far_keys[i] && vals[j] == so_far_vals[i])
                so_far_in_kv = true;
        }
        assert (so_far_in_kv);
        assert (kv_in_so_far);
    }
}

int main() {
    hash_table *table = hash_table_new(*hash_code_int, *equal_int);
    unsigned long *keys = NULL;
    unsigned long *vals = NULL;
    unsigned long num_elems = 0;
    char c;
    unsigned long *k;
    unsigned long *v;
    bool flag;
    while ((c = getchar()) != EOF) {
        switch (c) {
            case '+':
                k = malloc(sizeof(unsigned long));
                if (k == NULL)
                    return 0;
                if (scanf("%lu", k) <= 0)
                    return 0;
                v = malloc(sizeof(unsigned long));
                if (v == NULL)
                    return 0;
                if (scanf("%lu", v) <= 0)
                    return 0;
                printf("Adding %lu == %lu\n", *k, *v);
                hash_table_put(table, k, v);
                flag = false;
                for (unsigned long i = 0; i < num_elems; i++) {
                    if (keys[i] == *k) {
                        vals[i] = *v;
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    keys = realloc(keys, (num_elems + 1) * sizeof(unsigned long));
                    keys[num_elems] = *k;
                    vals = realloc(vals, (num_elems + 1) * sizeof(unsigned long));
                    vals[num_elems] = *v;
                    num_elems += 1;
                }
                break;
            case '.':
                k = malloc(sizeof(long));
                if (k == NULL)
                    return 0;
                if (scanf("%lu", k) <= 0)
                    return 0;
                printf("Getting %lu\n", *k);
                v = (unsigned long *) hash_table_get(table, k);
                flag = false;
                for (unsigned long i = 0; i < num_elems; i++) {
                    if (keys[i] == *k) {
                        assert (*v == vals[i]);
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    assert (v == NULL);
                    printf("Got %lu -> NULL\n", *k);
                    assert(!hash_table_contains(table, k));
                } else {
                    printf("Got %lu -> %lu\n", *k, *v);
                    assert(hash_table_contains(table, k));
                }
                break;
            case '-':
                k = malloc(sizeof(long));
                if (k == NULL)
                    return 0;
                if (scanf("%lu", k) <= 0)
                    return 0;
                printf("Removing %lu\n", *k);
                v = (unsigned long *) hash_table_remove(table, k);
                flag = false;
                for (unsigned long i = 0; i < num_elems; i++) {
                    if (keys[i] == *k) {
                        assert (*v == vals[i]);
                        vals[i] = vals[num_elems - 1];
                        keys[i] = keys[num_elems - 1];
                        num_elems -= 1;
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    assert (v == NULL);
                    printf("Removed %lu -> NULL\n", *k);
                } else {
                    printf("Removed %lu -> %lu\n", *k, *v);
                }
                break;
            case 's':
                k = malloc(sizeof(long));
                if (k == NULL)
                    return 0;
                if (scanf("%lu", k) <= 0)
                    return 0;
                printf("Getting size...\n");
                unsigned long size = hash_table_size(table);
                assert (size == num_elems);
                printf("Size: %lu\n", size);
                break;
            case 'i':
                do_iter_test(table, keys, vals, num_elems);
                break;
            case '\n':
                break;
            default:
                return 0;
        }
    }
}

#endif