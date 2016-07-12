#include <assert.h>
#include "structures.h"
#include "value.h"

#include "../base_util.h"
#include "../config.h"

#define CHECK_TYPE(v, tp) do {\
    /* assert((v)->type == (tp)); */ \
    if ((v)->type != (tp))\
        ERROR("Invalid type: '%i' != '%i'", (v)->type, (tp));\
    } while (0)


struct Value value_copy_stack(struct Value * b){
    struct Value a;
    if (b->type == STRING || b->type == KEYWORD) {
        a.data.str = value_as_string_or_kwd(b);
    } else if(b->type == FLOAT){
        a.data.fl = value_as_float(b);
    } else if(b->type == LONG){
        a.data.ln = value_as_long(b);
    } else if (b->type == BOOL){
        a.data.bl = value_as_bool(b);
    } else if (b->type == ARRAY){
        a.data.array = array_copy_heap(value_as_array(b));
    } else if (b->type == UNDEF){
        a.type = UNDEF;
    } else {
        ERROR("Unknown type: %c", a.type);
    }
    a.type = b->type;
    return a;
}

struct Value *value_copy_heap(struct Value * b){
    struct Value *a = malloc_or_die(sizeof(struct Value));
    if (b->type == STRING || b->type == KEYWORD) {
        a->data.str = value_as_string_or_kwd(b);
    } else if(b->type == FLOAT){
        a->data.fl = value_as_float(b);
    } else if(b->type == LONG){
        a->data.ln = value_as_long(b);
    } else if (b->type == BOOL){
        a->data.bl = value_as_bool(b);
    } else if (b->type == ARRAY){
        a->data.array = array_copy_heap(value_as_array(b));
    } else if (b->type == UNDEF){
        a->type = UNDEF;
    } else {
        ERROR("Unknown type: %c", a->type);
    }
    a->type = b->type;
    return a;
}

void value_copy_to(struct Value * a, struct Value * b){
#if 0
    if (a->type == STRING) {
        string_free(value_as_string(a));
    } else if (a->type == KEYWORD) {
        string_free(value_as_keyword(a));
    }else if (a->type == ARRAY) {
        array_free(value_as_array(a));
    }
#endif
    if(b->type == STRING || b->type == KEYWORD){
        struct String b_str = value_as_string_or_kwd(b);
        string_copy_to(&a->data.str, &b_str);
    } else if(b->type == FLOAT){
        a->data.fl = value_as_float(b);
    } else if(b->type == LONG){
        a->data.ln = value_as_long(b);
    } else if (b->type == BOOL){
        a->data.bl = value_as_bool(b);
    } else if (b->type == ARRAY){
        array_copy_to(value_as_array(a), value_as_array(b));
    } else if (b->type == UNDEF){
       // a->type == UNDEF;
    } else {
        ERROR("Unknown type: %c", a->type);
    }
    a->type = b->type;
}

float value_as_float(struct Value *v) {
    CHECK_TYPE(v, FLOAT);
    return v->data.fl;
}
bool value_as_bool(struct Value *v) {
    CHECK_TYPE(v, BOOL);
    return v->data.bl;
}
huo_int_t value_as_long(struct Value *v) {
    CHECK_TYPE(v, LONG);
    return v->data.ln;
}
struct String value_as_string_or_kwd(struct Value *v) {
    if (v->type == STRING) {
        return value_as_string(v);
    } else if (v->type == KEYWORD) {
        return value_as_keyword(v);
    } else {
        ERROR("Unknown type: %c", v->type);
    }
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

size_t length(struct Value a);

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

struct Value value_from_long(huo_int_t l) {
    struct Value v = {
          .type = LONG,
          .data.ln = l
    };
    return v;
}

struct Value value_from_string(struct String str) {
    struct Value v = {
          .type = STRING,
          .data.str = str
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

size_t length(struct Value a) {
    if (a.type == STRING) {
        return string_length(&a.data.str);
    } else if (a.type == ARRAY) {
        return a.data.array->size;
    } else {
        ERROR("Type error: value of type '%c' has no length property", a.type);
    }
}

unsigned long value_keyword_hash_code(void *value) {
    struct String word = value_as_keyword((struct Value *) value);
    return string_hash_code(&word);
}
bool value_keyword_equality(void *a, void *b) {
    struct String aword = value_as_keyword((struct Value *) a);
    struct String bword = value_as_keyword((struct Value *) b);
    return string_matches_heap(&aword, &bword);
}


bool value_equals_shallow(struct Value *a, struct Value *b) {
    if (a->type != b->type)
        return false;
    switch (a->type) {
        case ARRAY:
            return value_as_array(a) == value_as_array(b);
        case STRING:
            return string_matches_stack(value_as_string(a), value_as_string(b));
        case LONG:
            return value_as_long(a) == value_as_long(b);
        case FLOAT:
            return value_as_float(a) == value_as_float(b);
        case BOOL:
            return value_as_bool(a) == value_as_bool(b);
        case KEYWORD:
            return string_matches_stack(value_as_keyword(a), value_as_keyword(b));
        default:
            ERROR("Unknown type %i", a->type);
    }
}

void value_negate(struct Value *v) {
    if(v->type == FLOAT){
        v->data.fl = -value_as_float(v);
    } else if(v->type == LONG){
        v->data.ln = -value_as_long(v);
    } else {
        ERROR("Type error: value of type '%c' cannot be negated", v->type);
    }
}

void value_free_stack(struct Value v) {
    switch (v.type) {
        case ARRAY:
            array_free(v.data.array);
            break;
        case STRING:
        case KEYWORD:
            string_free_stack(v.data.str);
            break;
        case LONG:
        case FLOAT:
        case BOOL:
        case UNDEF:
            break;
        default:
            ERROR("Unknown type %i", v.type);
    }
    v.type = UNDEF;
}

void value_free(struct Value *v) {
    if (v != NULL) {
        value_free_stack(*v);
        free(v);
    }
}
