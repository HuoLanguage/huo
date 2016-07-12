#ifndef _BASE_UTIL_H
#define _BASE_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "structures/structures.h"
#include <execinfo.h>
#include <assert.h>
#include "config.h"

#define PASTE_HELPER(a,b) a ## b
#define PASTE(a,b) PASTE_HELPER(a,b)

#define WARN_ONCE(...) _WARN_ONCE(PASTE(warn_once_, __COUNTER__), __VA_ARGS__)
#define _WARN_ONCE(UNIQ, ...) do {\
    static bool UNIQ = false;\
    if (!UNIQ) {\
        UNIQ = true;\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
    }\
} while (0)

#define ASSERT_NOREC(...) _ASSERT_NOREC(PASTE(warn_once_, __COUNTER__), __VA_ARGS__)

#if defined(NDEBUG)
#define _ASSERT_NOREC(UNIQ, ...)
#else
#define _ASSERT_NOREC(UNIQ, ...) do {\
    static bool UNIQ = false;\
    if (!UNIQ) {\
        UNIQ = true;\
        assert (__VA_ARGS__);\
        UNIQ = false;\
    }\
} while (0)
#endif

/* Macro because it makes printf errors easier to detect at compile time */
#define ERROR(...) ERROR_AT(__FILE__, __func__, __LINE__, __VA_ARGS__)
#define ERROR_AT(FILE, FUNC, LINE, ...) do {\
    void * buffer[5];\
    char ** strings;\
    backtrace(buffer, 5);\
    strings = backtrace_symbols(buffer, 5);\
    for (int i = 0; i < 5; i++){\
        printf ("%s\n", strings[i]);\
    }\
    fprintf(stderr, "Error at %s:%s:%i: ", FILE, FUNC, LINE);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    /*assert(false);*/\
    exit(1);\
} while (0);

#define ARR_MALLOC(num_elem, elem_val) malloc_or_die(arr_malloc_size((num_elem), sizeof(elem_val)))

// Would be a function, but then So. Much. Casting.
#define RESIZE(ptr_to_arr, new_len) do {\
    if ((new_len) == 0) {\
        free((void *) ptr_to_arr);\
        (ptr_to_arr) = NULL;\
    } else {\
        (ptr_to_arr) = realloc((void *) (ptr_to_arr), arr_malloc_size((new_len), sizeof((ptr_to_arr)[0])));\
        if ((ptr_to_arr) == NULL) {\
            ERROR("Malloc failure!");\
        }\
    }\
} while (0)

void *malloc_or_die(size_t size);
size_t arr_malloc_size(size_t num, size_t size);
char *o_strdup(const char *str);
hash_table *push_scope(struct Scopes * scopes);
void pop_scope(struct Scopes * scopes);
void sub_vars(struct Value *v, struct Scopes *scopes, huo_depth_t max_depth);

#endif
