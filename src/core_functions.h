#ifndef _CORE_FUNCTIONS_H
#define _CORE_FUNCTIONS_H

#include "structures/structures.h"

void print(struct Value a);

struct Value add(struct Value a, struct Value b);
struct Value array_add(struct Value_array *a, struct Value_array *b);

struct Value mul(struct Value a, struct Value b);

struct Value sub(struct Value a, struct Value b);
struct Value array_sub(struct Value_array *a, struct Value_array *b);

struct Value divide(struct Value a, struct Value b);
struct Value long_divide(long a, long b);

struct Value concat(struct Value a, struct Value b);
struct Value array_concat(struct Value_array *a, struct Value_array *b);
struct Value string_concat(struct String a, struct String b);

struct Value not(struct Value a, struct Value b);

struct Value equals(struct Value a, struct Value b);

struct Value set(struct Value index, struct Value item, struct Value to_set);
struct Value_array *array_set(long index, struct Value item, struct Value_array *array);
struct String string_set(long index, struct String item, struct String string);

struct Value push(struct Value what, struct Value where);
struct Value_array *array_push(struct Value a, struct Value_array *arr);
struct String *string_push(struct String a, struct String str);

struct Value substring(struct Value start, struct Value end, struct Value what);
struct Value substring_ll(long start_i, long end_i, struct Value what);
//struct Value array_slice(long start, long end, struct Value_array what);
struct String *string_substring(long start, long end, struct String what);

struct Value split(struct Value sep, struct Value what);
struct Value string_split(struct String sep, struct String what);

struct Value greater_than(struct Value a, struct Value b);

struct Value index(struct Value index, struct Value v);

#endif
