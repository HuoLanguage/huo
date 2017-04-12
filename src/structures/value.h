#ifndef _VALUE_H
#define _VALUE_H

#include <inttypes.h>
#include "structures.h"
#include "string.h"

enum Value_type {
    FLOAT,
    BOOL,
    LONG,
    STRING,
    ARRAY,
    KEYWORD,
    AST,
    UNDEF
};

typedef struct huo_ast_t huo_ast;

union Data {
    bool bl;
    huo_int_t ln;
    float fl;
    struct String str;
    struct Value_array * array;
    huo_ast * ast;
};

struct Value {
    enum Value_type type;
    union Data data;
};

#define CHECK_TYPE(v, tp) do {\
    /* assert((v)->type == (tp)); */ \
    if ((v)->type != (tp))\
        ERROR("Invalid type: '%i' != '%i'", (v)->type, (tp));\
    } while (0)

float value_as_float(struct Value *v);
bool value_as_bool(struct Value *v);
huo_int_t value_as_long(struct Value *v);
struct String value_as_string_or_kwd(struct Value *v);
struct String value_as_string(struct Value *v);
struct Value_array *value_as_array(struct Value *v);
struct String value_as_keyword(struct Value *v);

struct Value value_from_float(float f);
struct Value value_from_bool(bool b);
struct Value value_from_long(huo_int_t l);
struct Value value_from_string(struct String str);
struct Value value_from_array(struct Value_array *arr);
struct Value value_from_keyword(struct String *str);
struct Value value_from_undef();

struct Value value_copy_stack(struct Value * b);
struct Value *value_copy_heap(struct Value * b);
void value_copy_to(struct Value * a, struct Value * b);

size_t length(struct Value a);

unsigned long value_keyword_hash_code(void *value);
bool value_keyword_equality(void *a, void *b);

struct String type_to_string(enum Value_type type);
bool value_equals_shallow(struct Value *a, struct Value *b);
void value_negate(struct Value *v);
void value_free(struct Value *v);
void value_free_stack(struct Value v);

#endif
