#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>
#include "structures/structures.h"
#include "base_util.h"
#include "core_functions.h"
#include "config.h"
#include "process_defs.h"
#include "constants.h"

bool __size_t_mul_overflow(size_t a, size_t b, size_t *res) {
#if defined(MUL_OVERFLOW)
    return MUL_OVERFLOW(a, b, res);
#elif SIZE_MAX <= (UINTMAX_MAX / SIZE_MAX)
    uintmax_t res_long = (uintmax_t) a * (uintmax_t) b;
    if (res_long > SIZE_MAX)
        return true;
    *res = (size_t) res_long;
    return false;
#elif defined(__UINT128__)
    if (sizeof(__UINT128__) >= sizeof(size_t) * 2) {
        __UINT128__ res_long = (__UINT128__) a * (__UINT128__) b;
        if (res_long > SIZE_MAX)
            return true;
        *res = (size_t) res_long;
        return false;
    } else {
        WARN_ONCE("size_t is HOW long?");
        WARN_ONCE("Using slow but portable overflow test")
    }
#else
#pragma message ("Using slow but portable overflow test")
#endif
    //Slow but portable
    size_t res_short = a * b;
    if ((a != 0) && ((res_short / a) != b))
        return true;
    *res = res_short;
    return false;
}

void *malloc_or_die(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL)
        ERROR("Malloc error");
    return ptr;
}

size_t arr_malloc_size(size_t num, size_t size) {
    size_t res;
    if (__size_t_mul_overflow(num, size, &res)) {
        ERROR("Overflow in array allocation!");
    }
    return res;
}

char *o_strdup(const char *str) {
    size_t len = strlen(str);
    char *dup = ARR_MALLOC(len + 1, char);
    strcpy(dup, str);
    return dup;
}

hash_table *push_scope(struct Scopes * scopes){
    RESIZE(scopes->scopes, scopes->size + 1);
    hash_table *h = hash_table_new(&string_hash_code_vv, &string_matches_vv);
    scopes->scopes[scopes->size] = h;
    scopes->size++;
    scopes->current++;
    return h;
}

void pop_scope(struct Scopes * scopes){
    assert (scopes->size > 0);
    scopes->size -= 1;
    scopes->current -= 1;
    //hash_table_free(scopes->scopes[scopes->size]);
    RESIZE(scopes->scopes, scopes->size);
}

struct Value sub_vars(struct Value *v, struct Scopes *scopes, huo_depth_t max_depth) {
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (v->type == ARRAY) {
        for (size_t i = 0; i < v->data.array->size; i++) {
            *(v->data.array->values[i]) = sub_vars(v->data.array->values[i], scopes, max_depth);
        }
    } else if (v->type == KEYWORD) {
        struct Value *w = NULL;
        struct String kwd = value_as_keyword(v);
        if ((w = get_letted_value(scopes, kwd)) != NULL) {
            v = w;
        } else if (string_matches_heap(&kwd, &true_const)) {
            return value_from_bool(true);
        } else if (string_matches_heap(&kwd, &false_const)) {
            return value_from_bool(false);
        } else {
            ERROR("Undefined variable: %s", string_to_chars(&kwd));
        }
    }
    return *v;
}
