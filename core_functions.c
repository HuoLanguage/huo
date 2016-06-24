#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "structures.h"
#include "constants.h"
#include "base_util.h"
#include "core_functions.h"

void print(struct Value a){
    if(a.type == 's'){
        printf("\"%s\"", a.data.str.body);
    }
    else if(a.type == 'k'){
        printf("%s", a.data.str.body);
    }
    else if(a.type == 'l') {
        printf("%ld", a.data.ln);
    }
    else if(a.type == 'f') {
        printf("%f", a.data.fl);
    }
    else if(a.type == 'b') {
        if(a.data.bl == bool_true){
            printf("True");
        }
        else if(a.data.bl == bool_false){
            printf("False");
        }
        else if(a.data.bl == bool_undefined){
            printf("Unknown");
        }
    }
    else if(a.type == 'a') {
        printf("[ ");
        for(int i = 0; i < a.data.array->size; i++){
            print(*a.data.array->values[i]);
            if(i < a.data.array->size-1){
              printf(", ");
            }
        }
        printf(" ]");
    }
    else if(a.type == 'u') {
        printf("undefined");
    }
}

struct Value add(struct Value a, struct Value b){
    if(a.type == 'l' && b.type == 'l'){
        a.data.ln = a.data.ln + b.data.ln;
    }
    else if(a.type == 'f' && b.type == 'f'){
        a.data.fl = a.data.fl + b.data.fl;
    }
    else if(a.type == 'f' && b.type == 'l'){
        a.data.fl = a.data.fl + (float)b.data.ln;
    }
    else if(a.type == 'l' && b.type == 'f'){
        a.data.fl = (float)a.data.ln + b.data.fl;
        a.type = 'f';
    }
    else if(a.type == 'a' && b.type == 'a'){
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
            copy_value_to(a.data.array->values[i], &result);
        }
    }
    return a;
}

struct Value mul(struct Value a, struct Value b){
    if(a.type == 'l' && b.type == 'l'){
        a.data.ln = a.data.ln * b.data.ln;
    }
    else if(a.type == 'f' && b.type == 'f'){
        a.data.fl = a.data.fl * b.data.fl;
    }
    else if(a.type == 'f' && b.type == 'l'){
        a.data.fl = a.data.fl * (float)b.data.ln;
    }
    else if(a.type == 'l' && b.type == 'f'){
        a.data.fl = (float)a.data.ln * b.data.fl;
        a.type = 'f';
    }
    return a;
}

struct Value sub(struct Value a, struct Value b){
    if(a.type == 'l' && b.type == 'l'){
        a.data.ln = a.data.ln - b.data.ln;
    }
    else if(a.type == 'f' && b.type == 'f'){
        a.data.fl = a.data.fl - b.data.fl;
    }
    else if(a.type == 'f' && b.type == 'l'){
        a.data.fl = a.data.fl - (float)b.data.ln;
    }
    else if(a.type == 'l' && b.type == 'f'){
        a.data.fl = (float)a.data.ln - b.data.fl;
        a.type = 'f';
    }
    return a;
}

struct Value divide(struct Value a, struct Value b){
    if(a.type == 'l' && b.type == 'l'){
        a.data.fl = (float)a.data.ln/(float)b.data.ln;
        a.type = 'f';
    }
    else if(a.type == 'f' && b.type == 'f'){
        a.data.fl = a.data.fl/b.data.fl;
    }
    else if(a.type == 'f' && b.type == 'l'){
        a.data.fl = a.data.fl/(float)b.data.ln;
    }
    else if(a.type == 'l' && b.type == 'f'){
        a.data.fl = (float)a.data.ln/b.data.fl;
        a.type = 'f';
    }
    a.type = 'f';
    return a;
}

struct Value concat(struct Value a, struct Value b){
    if (a.type != 's' || b.type != 's') {
        ERROR("Tried to concat %c and %c", a.type, b.type);
    }
    string_concat_to(&a.data.str, &b.data.str);
    return a;
}

struct Value not(struct Value a, struct Value b){
    if(a.type == 'f' && b.type == 'f'){
        a.type = 'b';
        if(a.data.fl == b.data.fl){
            a.data.bl = bool_false;
        } else {
            a.data.bl = bool_true;
        }
    }
    else if(a.type == 'l' && b.type == 'l'){
        a.type = 'b';
        if(a.data.ln == b.data.ln){
            a.data.bl = bool_false;
        } else {
            a.data.bl = bool_true;
        }
    }
    else if(a.type == 's' && b.type == 's'){
        a.type = 'b';
        if(string_matches(&a.data.str, &b.data.str)){
            a.data.bl = bool_false;
        } else {
            a.data.bl = bool_true;
        }
    } else {
        ERROR("Mismatched types: %c != %c", a.type, b.type);
    }
    return a;
}

struct Value equals(struct Value a, struct Value b){
    if(a.type == 'f' && b.type == 'f'){
        a.type = 'b';
        if(a.data.fl == b.data.fl){
            a.data.bl = bool_true;
        } else {
            a.data.bl = bool_false;
        }
    }
    else if(a.type == 'l' && b.type == 'l'){
        a.type = 'b';
        if(a.data.ln == b.data.ln){
            a.data.bl = bool_true;
        } else {
            a.data.bl = bool_false;
        }
    }
    else if(a.type == 's' && b.type == 's'){
        a.type = 'b';
        if(string_matches(&a.data.str, &b.data.str)){
            a.data.bl = bool_true;
        } else {
            a.data.bl = bool_false;
        }
    } else {
        ERROR("Mismatched types: %c != %c", a.type, b.type);
    }
    return a;
}

struct Value greater_than(struct Value a, struct Value b){
    if(a.type == 'f' && b.type == 'f'){
        a.type = 'b';
        if(a.data.fl > b.data.fl){
            a.data.bl = bool_true;
        } else {
            a.data.bl = bool_false;
        }
    }
    else if(a.type == 'l' && b.type == 'l'){
        a.type = 'b';
        if(a.data.ln > b.data.ln){
            a.data.bl = bool_true;
        } else {
            a.data.bl = bool_false;
        }
    }
    else {
        ERROR("Mismatched types: %c != %c", a.type, b.type);
    }
    return a;
}

struct Value length(struct Value a){
    if (a.type == 's') {
        assert(string_is_sane(&a.data.str));
    }
    if(a.type != 'a' && a.type != 's'){
      ERROR("Type error: value has no length property");
/*      a.type = 'u';
      return a; */
    } else {
      struct Value length = {
          .type = 'l',
          .data = {
            .ln= a.type=='a'?(long)a.data.array->size:a.data.str.length
          }
      };
      return length;
    }
}

struct Value array_index(struct Value a, struct Value list){
    if(list.type == 'a'){
        return *list.data.array->values[a.data.ln];
    }
    else if(list.type == 's'){
        assert(string_is_sane(&list.data.str));
        struct Value result = {
            .type='s',
            .data={
                .str={
                    .length=1
                }
            }
        };
        result.data.str.body = malloc(sizeof(char) + 1);
        if (result.data.str.body == NULL) {
            ERROR("Malloc failure");
        }
            
        assert(string_is_sane(&result.data.str));
        result.data.str.body[0] = list.data.str.body[a.data.ln];
        result.data.str.body[1] = 0;
        return result;
    } else {
        ERROR("Index takes a number and a string or array, but got ('%c' != 'l', '%c' != ['a'|'s']).", a.type, list.type);
    }
}

struct Value array_set(struct Value index, struct Value item, struct Value array){
    if (index.type != 'l') {
        ERROR("Set index type invalid:  ('%c' != 'l')", index.type);
    }
    if (array.type != 'a') {
        ERROR("Set array type invalid:  ('%c' != 'a')", array.type);
    }
    int idx = (int) index.data.ln;
    if(idx > array.data.array->size-1){
        array.data.array->size = idx+1;
    }
    array.data.array->values[idx] = copy_value_heap(&item);
    return array;
}

struct Value array_push(struct Value a, struct Value arr){
    if (arr.type != 'a') {
        ERROR("Push takes an item and an array, but got ('%c' != 'a').", arr.type);
    }
    arr.data.array->values[arr.data.array->size] = copy_value_heap(&a);
    arr.data.array->size++;
    return arr;
}

struct Value substring(int start, int end, struct Value str){
    assert(string_is_sane(&str.data.str));
    struct Value result;
    result.type = 's';
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
            free(result.data.str.body);
            result.data.str.body = NULL;
        }
        assert(string_is_sane(&result.data.str));
        return result;
    }
}

struct Value split_string(struct Value a, struct Value str){
    assert(string_is_sane(&a.data.str));
    assert(string_is_sane(&str.data.str));
    int indexes[1000];
    int counter = 0;
    for(int i = 0; i < str.data.str.length; i++){
        if(a.data.str.body[0] == str.data.str.body[i]){
            indexes[counter] = i;
            counter++;
        }
    }
    struct Value result;
    result.type = 'a';
    struct Value_array * array = malloc(sizeof(struct Value_array));
    array->size = 0;
    for(int l = 0; l <= counter; l++){
        int start = !l ? l : indexes[l-1] + 1;
        int end = (l < counter) ? indexes[l] : str.data.str.length;
        
        struct Value item = substring(start, end, str);
        
        array->values[array->size] = copy_value_heap(&item);
        array->size++;
    }
    result.data.array = array;
    return result;
}
