#include "structures.h"
#include "value.h"

#include "../base_util.h"

struct Value value_from_long(long l) {
    struct Value v = {
          .type = LONG,
          .data.ln = l
    };
    return v;
}

long length(struct Value a) {
    if (a.type == STRING) {
        return string_length(&a.data.str);
    } else if (a.type == ARRAY) {
        return (long) a.data.array->size;
    } else {
        ERROR("Type error: value of type '%c' has no length property", a.type);
    }
}

struct Value index(struct Value a, struct Value list) {
    long i = a.data.ln;
    if(list.type == ARRAY){
        if (i < 0) {
            ERROR("Negative index: %li", i);
        }
        long len = length(list);
        if (i >= len) {
            ERROR("Invalid index: %li (len %li)", i, len);
        }
        return *list.data.array->values[i];
    }
    else if(list.type == STRING){
        struct Value result = {
            .type=STRING,
            .data={
                .str=string_from_char(string_index(&list.data.str, i))
            }
        };
        return result;
    } else {
        ERROR("Index takes a number and a string or array, but got ('%c' != LONG, '%c' != [ARRAY|STRING]).", a.type, list.type);
    }
}