#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "structures/structures.h"
#include "constants.h"
#include "base_util.h"
#include "core_functions.h"
#include "config.h"

void print(struct Value a){
    if(a.type == STRING){
        struct String str = value_as_string(&a);
        printf("\"%s\"", string_to_chars(&str));
    }
    else if(a.type == KEYWORD){
        struct String kwd = value_as_keyword(&a);
        printf("%s", string_to_chars(&kwd));
    }
    else if(a.type == LONG) {
        printf("%ld", value_as_long(&a));
    }
    else if(a.type == FLOAT) {
        printf("%f", value_as_float(&a));
    }
    else if(a.type == BOOL) {
        if (value_as_bool(&a)){
            printf("True");
        }
        else{
            printf("False");
        }
    }
    else if(a.type == ARRAY) {
        printf("[ ");
        struct Value_array *arr = value_as_array(&a);
        for(int i = 0; i < arr->size; i++){
            print(*arr->values[i]);
            if(i < arr->size-1){
              printf(", ");
            }
        }
        printf(" ]");
    }
    else if(a.type == UNDEF) {
        printf("undefined");
    }
}

struct Value add(struct Value a, struct Value b){
    if(a.type == LONG && b.type == LONG){
        return value_from_long(value_as_long(&a) + value_as_long(&b));
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        return value_from_float(value_as_float(&a) + value_as_float(&b));
    }
    else if(a.type == FLOAT && b.type == LONG){
        return value_from_float(value_as_float(&a) + (float)value_as_long(&b));
    }
    else if(a.type == LONG && b.type == FLOAT){
        return value_from_float((float)value_as_long(&a) + value_as_float(&b));
    }
    else {
        ERROR("Mismatched types for add: %d != %d", a.type, b.type);
    }
}

struct Value mul(struct Value a, struct Value b){
    if(a.type == LONG && b.type == LONG){
        return value_from_long(value_as_long(&a) * value_as_long(&b));
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        return value_from_float(value_as_float(&a) * value_as_float(&b));
    }
    else if(a.type == FLOAT && b.type == LONG){
        return value_from_float(value_as_float(&a) * (float)value_as_long(&b));
    }
    else if(a.type == LONG && b.type == FLOAT){
        return value_from_float((float)value_as_long(&a) * value_as_float(&b));
    }
    else {
        ERROR("Mismatched types for add: %d != %d", a.type, b.type);
    }
}

struct Value sub(struct Value a, struct Value b){
    if(a.type == LONG && b.type == LONG){
        return value_from_long(value_as_long(&a) - value_as_long(&b));
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        return value_from_float(value_as_float(&a) - value_as_float(&b));
    }
    else if(a.type == FLOAT && b.type == LONG){
        return value_from_float(value_as_float(&a) - (float)value_as_long(&b));
    }
    else if(a.type == LONG && b.type == FLOAT){
        return value_from_float((float)value_as_long(&a) - value_as_float(&b));
    }
    else {
        ERROR("Mismatched types for sub: %d != %d", a.type, b.type);
    }
}

struct Value divide(struct Value a, struct Value b) {
    if(a.type == LONG && b.type == LONG){
        return long_divide(value_as_long(&a), value_as_long(&b));
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        return value_from_float(value_as_float(&a) / value_as_float(&b));
    }
    else if(a.type == FLOAT && b.type == LONG){
        return value_from_float(value_as_float(&a) / (float) value_as_long(&b));
    }
    else if(a.type == LONG && b.type == FLOAT){
        return value_from_float((float) value_as_long(&a) / value_as_float(&b));
    }
    else {
        ERROR("Mismatched types for divide: %d != %d", a.type, b.type);
    }
}

//
struct Value long_divide(long a, long b) {
    if (a % b == 0) {
        return value_from_long(a / b);
    } else {
        return value_from_float((float) a /  (float) b);
    }
}

struct Value concat(struct Value a, struct Value b){
    if (a.type == STRING && b.type == STRING) {
        return string_concat(value_as_string(&a), value_as_string(&b));
    } else if (a.type == ARRAY && b.type == ARRAY) {
        return array_concat(value_as_array(&a), value_as_array(&b));
    } else {
        ERROR("Tried to concat %c and %c", a.type, b.type);
    }
}

struct Value array_concat(struct Value_array *a, struct Value_array *b) {
    return value_from_array(array_concat_heap(a, b));
}
struct Value string_concat(struct String a, struct String b) {
    return value_from_string(string_concat_heap(&a, &b));
}

struct Value not(struct Value a, struct Value b){
    if(a.type == BOOL && b.type == BOOL){
        return value_from_bool(value_as_bool(&a) != value_as_bool(&b));
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        return value_from_bool(value_as_float(&a) != value_as_float(&b));
    }
    else if(a.type == LONG && b.type == LONG){
        return value_from_bool(value_as_long(&a) != value_as_long(&b));
    }
    else if(a.type == STRING && b.type == STRING){
        return value_from_bool(!string_matches_stack(value_as_string(&a), value_as_string(&b)));
    }
    else if(a.type == ARRAY && b.type == ARRAY){
        return value_from_bool(!array_matches(value_as_array(&a), value_as_array(&b)));
    }
    else {
        ERROR("Mismatched types: %d != %d", a.type, b.type);
    }
}

struct Value equals(struct Value a, struct Value b){
    if(a.type == BOOL && b.type == BOOL){
        return value_from_bool(value_as_bool(&a) == value_as_bool(&b));
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        return value_from_bool(value_as_float(&a) == value_as_float(&b));
    }
    else if(a.type == LONG && b.type == LONG){
        return value_from_bool(value_as_long(&a) == value_as_long(&b));
    }
    else if(a.type == STRING && b.type == STRING){
        return value_from_bool(string_matches_stack(value_as_string(&a), value_as_string(&b)));
    }
    else if(a.type == ARRAY && b.type == ARRAY){
        return value_from_bool(array_matches(value_as_array(&a), value_as_array(&b)));
    }
    else {
        ERROR("Mismatched types: %d != %d", a.type, b.type);
    }
}

struct Value greater_than(struct Value a, struct Value b){
    if(a.type == BOOL && b.type == BOOL){
        return value_from_bool(value_as_bool(&a) > value_as_bool(&b));
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        return value_from_bool(value_as_float(&a) > value_as_float(&b));
    }
    else if(a.type == LONG && b.type == LONG){
        return value_from_bool(value_as_long(&a) > value_as_long(&b));
    }
    else {
        ERROR("Mismatched types: %d != %d", a.type, b.type);
    }
}


struct Value set(struct Value index_val, struct Value item, struct Value to_set) {
    long index = value_as_long(&index_val);

    if (to_set.type == ARRAY) {
        return value_from_array(array_set(index, item, value_as_array(&to_set)));
    } else if (to_set.type == STRING) {
        struct String set = string_set(index, value_as_string(&item), value_as_string(&to_set));
        return value_from_string(string_copy_heap(&set));
    } else {
        ERROR("Set type invalid:  ('%c' != [ARRAY, STRING])", to_set.type);
    }
}

struct Value_array *array_set(long index, struct Value item, struct Value_array *array) {

    if (index < 0) {
        ERROR("Invalid array index: %li", index);
    }
    // Have to copy before incrementing size or else
    // recursive copies segfault
    struct Value *val = value_copy_heap(&item);

    if (index >= array->size) {
        RESIZE(array->values, index + 1);
        struct Value undef = {.type=UNDEF};
        for (long i = array->size; i < index; i++) {
            array->values[i] = value_copy_heap(&undef);
        }
        array->size = index + 1;
    }
    array->values[index] = val;
    return array;
}

struct String string_set(long index, struct String item, struct String string){
    if (index < 0) {
        ERROR("Invalid string index: %li", index);
    }
    char c = string_index(&item, 0);
    if (index == string_length(&string)) {
        struct String char_str = string_from_char(c);
        string_concat_to(&string, &char_str);
    } else if (index < string_length(&string)) {
        string.body[index] = c;
    } else {
        ERROR("Invalid index: %li > %li", index, string_length(&string));
    }
    return string;
}

struct Value push(struct Value what, struct Value where) {
    if (where.type == ARRAY) {
        return value_from_array(array_push(what, value_as_array(&where)));
    } else if (where.type == STRING) {
        return value_from_string(string_push(value_as_string(&what), value_as_string(&where)));
    } else {
        ERROR("Push type invalid:  ('%c' != [ARRAY, STRING])", where.type);
    }
}

struct Value_array *array_push(struct Value a, struct Value_array *array){
    RESIZE(array->values, array->size + 1);
    array->values[array->size] = value_copy_heap(&a);
    array->size++;
    return array;
}

struct String *string_push(struct String a, struct String str){
    if (string_length(&a) != 1) {
        ERROR("Character does not have length 1");
    }
    return string_concat_heap(&str, &a);
}

struct Value substring(struct Value start, struct Value end, struct Value what) {
    long start_i = value_as_long(&start);
    long end_i = value_as_long(&end);
    return substring_ll(start_i, end_i, what);
}

struct Value substring_ll(long start_i, long end_i, struct Value what) {
    //if (what.type == ARRAY) {
    //    return value_from_array(array_slice(start_i, end_i, value_as_array(&what)));
    //} else
    if (what.type == STRING) {
        return value_from_string(string_substring(start_i, end_i, value_as_string(&what)));
    } else {
        ERROR("Substring type invalid:  ('%c' != [ARRAY, STRING])", what.type);
    }
}

struct String *string_substring(long start, long end, struct String str){
    assert(string_is_sane(&str));
    if(start < 0 || start > string_length(&str)) {
        ERROR("String start index out of range for substring: should be 0 <= %li < %li", start, string_length(&str));
    }
    else if (end < 0 || end > string_length(&str)){
        ERROR("String end index out of range for substring: should be 0 <= %li < %li", end, string_length(&str));
    } else {
        if (end > start) {
            struct String *result = malloc_or_die(sizeof(struct String));
            result->length = (end - start),
            result->body = ARR_MALLOC(string_length(&str) + 1, char);
            for(int i = 0; i < result->length; i++){
                result->body[i] = str.body[i + start];
            }
            result->body[result->length] = 0;
            assert(string_is_sane(result));
            return result;
        } else {
            struct String null_str = string_from_chars(NULL);
            return string_copy_heap(&null_str);
        }
    }
}

struct Value split(struct Value sep, struct Value what) {
    return string_split(value_as_string(&sep), value_as_string(&what));
}

struct Value string_split(struct String sep, struct String what) {

    assert(string_is_sane(&sep));
    assert(string_is_sane(&what));

    int c = 0;
    for(int i = 0; i < what.length; i++){
        if(sep.length == 0 || sep.body[0] == what.body[i]){
            c += 1;
        }
    }
    int *indexes = NULL;
    RESIZE(indexes, c);
    int d = 0;
    for(int i = 0; i < what.length; i++){
        if(sep.length == 0 || sep.body[0] == what.body[i]){
            indexes[d++] = i;
        }
    }
    assert (c == d);
    struct Value_array * array = malloc_or_die(sizeof(struct Value_array));
    array->values = NULL;
    RESIZE(array->values, c + 1);
    array->size = 0;
    for(int l = 0; l <= c; l++){
        int start = !l ? l : indexes[l-1] + 1;
        int end = (l < c) ? indexes[l] : what.length;

        struct Value item = value_from_string(string_substring(start, end, what));

        array->values[array->size] = value_copy_heap(&item);
        array->size++;
    }
    return value_from_array(array);
}


struct Value index(struct Value index, struct Value list) {
    long i = value_as_long(&index);
    if(list.type == ARRAY){
        struct Value_array *a = value_as_array(&list);
        if (i < 0) {
            ERROR("Negative index: %li", i);
        }
        long len = a->size;
        if (i >= len) {
            ERROR("Invalid index: %li (len %li)", i, len);
        }
        return *a->values[i];
    }
    else if(list.type == STRING){
        struct String s = value_as_string(&list);
        struct String indexed_char = string_from_char(string_index(&s, i));
        return value_from_string(string_copy_heap(&indexed_char));
    } else {
        ERROR("Index takes a string or array, but got '%c' != [ARRAY|STRING]).", list.type);
    }
}
