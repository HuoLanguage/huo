#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "constants.h"
#include "base_util.h"

void print(struct Value a){
    if(a.type == 's'){
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
        if(string_matches(a.data.str, b.data.str)){
            a.data.bl = bool_false;
        } else {
            a.data.bl = bool_true;
        }
    } else {
        printf("Error: mismatched types for values: ");
        print(a);
        printf(" and ");
        print(b);
        printf("in not function.\n");
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
        if(string_matches(a.data.str, b.data.str)){
            a.data.bl = bool_true;
        } else {
            a.data.bl = bool_false;
        }
    } else {
        printf("Error: mismatched types for values: ");
        print(a);
        printf(" and ");
        print(b);
        printf(" in equals function.\n");
    }
    return a;
}
