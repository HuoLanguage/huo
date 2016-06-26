#ifndef _VALUE_H
#define _VALUE_H

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
    char bl;
    long ln;
    float fl;
    struct String str;
    struct Value_array * array;
};

struct Value {
    enum Value_type type;
    union Data data;
};



struct Value value_from_long(long l);
long length(struct Value a);
struct Value index(struct Value a, struct Value arr);

#endif