#include "structures.h"
#include "value.h"

#include "../base_util.h"

#define CHECK_TYPE(v, tp) do {\
    if ((v)->type != (tp))\
        ERROR("Invalid type: '%i' != '%i'", (v)->type, (tp));\
    } while (0)

float value_as_float(struct Value *v) {
    CHECK_TYPE(v, FLOAT);
    return v->data.fl;
}
bool value_as_bool(struct Value *v) {
    CHECK_TYPE(v, BOOL);
    return v->data.bl;
}
long value_as_long(struct Value *v) {
    CHECK_TYPE(v, LONG);
    return v->data.ln;
}
struct String value_as_string(struct Value *v) {
    CHECK_TYPE(v, STRING);
    return v->data.str;
}
struct Value_array *value_as_array(struct Value *v) {
    CHECK_TYPE(v, ARRAY);
    return v->data.array;
}
struct String value_as_keyword(struct Value *v) {
    CHECK_TYPE(v, KEYWORD);
    return v->data.str;
}



long length(struct Value a);
struct Value index(long index, struct Value arr);

struct Value value_from_float(float f) {
    struct Value v = {
          .type = FLOAT,
          .data.fl = f
    };
    return v;
}

struct Value value_from_bool(bool b) {
    struct Value v = {
          .type = BOOL,
          .data.bl = b
    };
    return v;
}

struct Value value_from_long(long l) {
    struct Value v = {
          .type = LONG,
          .data.ln = l
    };
    return v;
}

struct Value value_from_string(struct String *str) {
    struct Value v = {
          .type = STRING,
          .data.str = *str
    };
    return v;
}

struct Value value_from_array(struct Value_array *arr) {
    struct Value v = {
          .type = ARRAY,
          .data.array = arr
    };
    return v;
}

struct Value value_from_keyword(struct String *kwd) {
     struct Value v = {
          .type = KEYWORD,
          .data.str = *kwd
    };
    return v;
}
struct Value value_from_undef() {
    struct Value v = {
          .type = UNDEF,
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

struct Value index(long i, struct Value list) {
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
        ERROR("Index takes a string or array, but got '%c' != [ARRAY|STRING]).", list.type);
    }
}