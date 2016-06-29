#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "structures/structures.h"
#include "constants.h"
#include "base_util.h"
#include "core_functions.h"

void print(struct Value a){
    if(a.type == STRING){
        printf("\"%s\"", string_to_chars(&a.data.str));
    }
    else if(a.type == KEYWORD){
        printf("%s", string_to_chars(&a.data.str));
    }
    else if(a.type == LONG) {
        printf("%ld", a.data.ln);
    }
    else if(a.type == FLOAT) {
        printf("%f", a.data.fl);
    }
    else if(a.type == BOOL) {
        if (a.data.bl){
            printf("True");
        }
        else{
            printf("False");
        }
    }
    else if(a.type == ARRAY) {
        printf("[ ");
        for(int i = 0; i < a.data.array->size; i++){
            print(*a.data.array->values[i]);
            if(i < a.data.array->size-1){
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
        a.data.ln = a.data.ln + b.data.ln;
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        a.data.fl = a.data.fl + b.data.fl;
    }
    else if(a.type == FLOAT && b.type == LONG){
        a.data.fl = a.data.fl + (float)b.data.ln;
    }
    else if(a.type == LONG && b.type == FLOAT){
        a.data.fl = (float)a.data.ln + b.data.fl;
        a.type = FLOAT;
    }
    else if(a.type == ARRAY && b.type == ARRAY){
        return array_add(a, b);
    }
    return a;
}

struct Value array_add(struct Value a, struct Value b){
    if(a.data.array->size != b.data.array->size){
        ERROR("Tried to add arrays of different sizes: %i != %i", a.data.array->size, b.data.array->size);
    } else {
        for(int i = 0; i < a.data.array->size; i++){
            struct Value result = add(*a.data.array->values[i], *b.data.array->values[i]);
            value_copy_to(a.data.array->values[i], &result);
        }
    }
    return a;
}

struct Value mul(struct Value a, struct Value b){
    if(a.type == LONG && b.type == LONG){
        a.data.ln = a.data.ln * b.data.ln;
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        a.data.fl = a.data.fl * b.data.fl;
    }
    else if(a.type == FLOAT && b.type == LONG){
        a.data.fl = a.data.fl * (float)b.data.ln;
    }
    else if(a.type == LONG && b.type == FLOAT){
        a.data.fl = (float)a.data.ln * b.data.fl;
        a.type = FLOAT;
    }
    return a;
}

struct Value sub(struct Value a, struct Value b){
    if(a.type == LONG && b.type == LONG){
        a.data.ln = a.data.ln - b.data.ln;
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        a.data.fl = a.data.fl - b.data.fl;
    }
    else if(a.type == FLOAT && b.type == LONG){
        a.data.fl = a.data.fl - (float)b.data.ln;
    }
    else if(a.type == LONG && b.type == FLOAT){
        a.data.fl = (float)a.data.ln - b.data.fl;
        a.type = FLOAT;
    }
    return a;
}

struct Value divide(struct Value a, struct Value b){
    if(a.type == LONG && b.type == LONG){
        a.data.fl = (float)a.data.ln/(float)b.data.ln;
        a.type = FLOAT;
    }
    else if(a.type == FLOAT && b.type == FLOAT){
        a.data.fl = a.data.fl/b.data.fl;
    }
    else if(a.type == FLOAT && b.type == LONG){
        a.data.fl = a.data.fl/(float)b.data.ln;
    }
    else if(a.type == LONG && b.type == FLOAT){
        a.data.fl = (float)a.data.ln/b.data.fl;
        a.type = FLOAT;
    }
    a.type = FLOAT;
    return a;
}

struct Value concat(struct Value a, struct Value b){
    if (a.type != STRING || b.type != STRING) {
        ERROR("Tried to concat %c and %c", a.type, b.type);
    }
    string_concat_to(&a.data.str, &b.data.str);
    return a;
}

struct Value not(struct Value a, struct Value b){
    if(a.type == FLOAT && b.type == FLOAT){
        a.type = BOOL;
        if(a.data.fl == b.data.fl){
            a.data.bl = false;
        } else {
            a.data.bl = true;
        }
    }
    else if(a.type == LONG && b.type == LONG){
        a.type = BOOL;
        if(a.data.ln == b.data.ln){
            a.data.bl = false;
        } else {
            a.data.bl = true;
        }
    }
    else if(a.type == STRING && b.type == STRING){
        a.type = BOOL;
        if(string_matches(&a.data.str, &b.data.str)){
            a.data.bl = false;
            printf("%c\n", false);
        } else {
            a.data.bl = true;
        }
    } else {
        ERROR("Mismatched types: %d != %d", a.type, b.type);
    }
    return a;
}

struct Value equals(struct Value a, struct Value b){
    if(a.type == FLOAT && b.type == FLOAT){
        a.type = BOOL;
        if(a.data.fl == b.data.fl){
            a.data.bl = true;
        } else {
            a.data.bl = false;
        }
    }
    else if(a.type == LONG && b.type == LONG){
        a.type = BOOL;
        if(a.data.ln == b.data.ln){
            a.data.bl = true;
        } else {
            a.data.bl = false;
        }
    }
    else if(a.type == STRING && b.type == STRING){
        a.type = BOOL;
        if(string_matches(&a.data.str, &b.data.str)){
            a.data.bl = true;
        } else {
            a.data.bl = false;
        }
    } else {
        ERROR("Mismatched types: %c != %c", a.type, b.type);
    }
    return a;
}

struct Value greater_than(struct Value a, struct Value b){
    if(a.type == FLOAT && b.type == FLOAT){
        a.type = BOOL;
        if(a.data.fl > b.data.fl){
            a.data.bl = true;
        } else {
            a.data.bl = false;
        }
    }
    else if(a.type == LONG && b.type == LONG){
        a.type = BOOL;
        if(a.data.ln > b.data.ln){
            a.data.bl = true;
        } else {
            a.data.bl = false;
        }
    }
    else {
        ERROR("Mismatched types: %c != %c", a.type, b.type);
    }
    return a;
}

struct Value array_set(struct Value index, struct Value item, struct Value array){
    if (index.type != LONG) {
        ERROR("Set index type invalid:  ('%c' != LONG)", index.type);
    }
    if (array.type != ARRAY) {
        ERROR("Set array type invalid:  ('%c' != ARRAY)", array.type);
    }
    // Have to copy before incrementing size or else
    // recursive copies segfault
    struct Value *val = value_copy_heap(&item);
    int idx = (int) index.data.ln;
    if (idx < 0) {
        ERROR("Invalid array index: %i", idx);
    }
    if (idx >= array.data.array->size) {
        while (array.data.array->size < idx) {
            struct Value undef = {.type=UNDEF};
            array_push(undef, array);
        }
        array_push(*val, array); // double-copy, but oh well.
    } else {
        array.data.array->values[idx] = val;
    }
    return array;
}

struct Value string_set(struct Value index, struct Value item, struct Value string){
    long idx = index.data.ln;
    string.data.str.body[idx] = item.data.str.body[0];
    return string;
}

struct Value array_push(struct Value a, struct Value arr){
    if (arr.type != ARRAY) {
        ERROR("Push takes an item and an array, but got ('%c' != ARRAY).", arr.type);
    }
    arr.data.array->values[arr.data.array->size] = value_copy_heap(&a);
    arr.data.array->size++;
    return arr;
}

struct Value substring(int start, int end, struct Value str){
    assert(string_is_sane(&str.data.str));
    struct Value result;
    result.type = STRING;
    if(start < 0 || start > str.data.str.length) {
        ERROR("String start index out of range for substring: should be 0 <= %i < %i", start, str.data.str.length);
    }
    else if (end < 0 || end > str.data.str.length){
        ERROR("String end index out of range for substring: should be 0 <= %i < %i", end, str.data.str.length);
    } else {
        if (end > start) {
            result.data.str.length = (end - start);
            result.data.str.body = malloc(sizeof(char) * (result.data.str.length + 1));
            if (result.data.str.body == NULL) {
                ERROR("Malloc failure");
            }
            for(int i = 0; i < result.data.str.length; i++){
                result.data.str.body[i] = str.data.str.body[i + start];
            }
            result.data.str.body[result.data.str.length] = 0;
        } else {
            result.data.str.length = 0;
            result.data.str.body = NULL;
        }
        assert(string_is_sane(&result.data.str));
        return result;
    }
}

struct Value split_string(struct Value a, struct Value str){
    if (a.type != STRING || str.type != STRING) {
        ERROR("Split takes two strings, but got ('%c' != STRING, '%c' != STRING).", a.type, str.type);
    }
    assert(string_is_sane(&a.data.str));
    assert(string_is_sane(&str.data.str));
    int indexes[1000];
    int counter = 0;
    for(int i = 0; i < str.data.str.length; i++){
        if(a.data.str.length == 0 || a.data.str.body[0] == str.data.str.body[i]){
            indexes[counter] = i;
            counter++;
        }
    }
    struct Value result;
    result.type = ARRAY;
    struct Value_array * array = malloc(sizeof(struct Value_array));
    array->size = 0;
    for(int l = 0; l <= counter; l++){
        int start = !l ? l : indexes[l-1] + 1;
        int end = (l < counter) ? indexes[l] : str.data.str.length;

        struct Value item = substring(start, end, str);

        array->values[array->size] = value_copy_heap(&item);
        array->size++;
    }
    result.data.array = array;
    return result;
}
