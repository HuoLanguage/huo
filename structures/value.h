#ifndef _VALUE_H
#define _VALUE_H

#include "structures.h"

enum Value_type {
    FLOAT,
    BOOL,
    LONG,
    STRING,
    ARRAY,
    KEYWORD,
    UNDEF
};

union Data {
    bool bl;
    long ln;
    float fl;
    struct String str;
    struct Value_array * array;
};

struct Value {
    enum Value_type type;
    union Data data;
};

float value_as_float(struct Value *v);
bool value_as_bool(struct Value *v);
long value_as_long(struct Value *v);
struct String value_as_string(struct Value *v);
struct Value_array *value_as_array(struct Value *v);
struct String value_as_keyword(struct Value *v);

struct Value value_from_float(float f);
struct Value value_from_bool(bool b);
struct Value value_from_long(long l);
struct Value value_from_string(struct String *str);
struct Value value_from_array(struct Value_array *arr);
struct Value value_from_keyword(struct String *str);
struct Value value_from_undef();

long length(struct Value a);
struct Value index(long index, struct Value arr);

#endif