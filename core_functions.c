#include <stdlib.h>
#include <stdio.h>
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
            copy_value(a.data.array->values[i], &result);
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
    int len = a.data.str.length;
    a.data.str.length += b.data.str.length;
    for(int l = 0; l < b.data.str.length; l++){
        a.data.str.body[l+len] = b.data.str.body[l];
    }
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
        struct Value result = {
            .type='s',
            .data={
                .str={
                    .length=1
                }
            }
        };
        result.data.str.body[0] = list.data.str.body[a.data.ln];
        return result;
    } else {
        ERROR("Index takes a number and a string or array, but got ('%c' != 'l', '%c' != ['a'|'s']).", a.type, list.type);
    }
}

struct Value array_set(struct Value index, struct Value item, struct Value array){
    int idx = (int) index.data.ln;
    if(idx > array.data.array->size-1){
        struct Value * val = malloc(sizeof(struct Value));
        array.data.array->values[idx] = val;
        array.data.array->size = idx+1;
    }
    copy_value(array.data.array->values[idx], &item);
    return array;
}

struct Value array_push(struct Value a, struct Value arr){
    struct Value * val = malloc(sizeof(struct Value));
    copy_value(val, &a);
    arr.data.array->values[arr.data.array->size] = val;
    arr.data.array->size++;
    return arr;
}

struct Value substring(int start, int end, struct Value str){
    struct Value result;
    result.type = 's';
    if(start < 0 || start > str.data.str.length) {
        ERROR("String start index out of range for substring: should be 0 <= %i < %i", start, str.data.str.length);
    }
    else if (end < 0 || end > str.data.str.length){
        ERROR("String end index out of range for substring: should be 0 <= %i < %i", end, str.data.str.length);
    } else {
        struct String new_string;
        if (end > start) {
            new_string.length = (end - start);
        } else {
            new_string.length = 0;
        }
        for(int i = 0; i < new_string.length; i++){
            new_string.body[i] = str.data.str.body[i + start];
        }
        result.data.str = new_string;
        return result;
    }
}

struct Value split_string(struct Value a, struct Value str){
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

        struct Value * next_item = malloc(sizeof(struct Value));
        struct Value item = substring(start, end, str);

        copy_value(next_item, &item);
        array->values[array->size] = next_item;
        array->size++;
    }
    result.data.array = array;
    return result;
}
