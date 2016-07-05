#ifndef HUO_ARRAY_H
#define HUO_ARRAY_H 1

#include <stdbool.h>
#include <stddef.h>

struct Value_array {
    size_t size;
    struct Value **values;
};

void array_copy_to(struct Value_array *to, struct Value_array *from);
struct Value_array array_copy_stack(struct Value_array *arr);
struct Value_array *array_copy_heap(struct Value_array *arr);
struct Value_array *array_concat_heap(struct Value_array *a, struct Value_array *b);
void array_concat_to(struct Value_array *to, struct Value_array *from);
bool array_matches(struct Value_array *base, struct Value_array *compare);

#endif
