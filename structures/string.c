#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../base_util.h"
#include "string.h"


bool string_is_sane(struct String *s) {
    assert (s != NULL);
    assert(s->length == 0 || s->body != NULL);
    assert(s->length == 0 || s->body[s->length] == 0);
    return true;
}

/*
struct String string_new(char *str) {
    struct String s = {
        .length = strlen(str);
        .data=str;
    }
    assert(string_is_sane(s));
    return s;
}*/

void string_concat_to(struct String *to, struct String *from) {
    assert(string_is_sane(from));
    assert(string_is_sane(to));
    int len = to->length;
    to->length += from->length;
    RESIZE(to->body, to->length + 1);
    for(int l = 0; l < from->length; l++){
        to->body[l+len] = from->body[l];
    }
    to->body[to->length] = 0;
    assert(string_is_sane(from));
    assert(string_is_sane(to));
}

int string_contains(char ch, struct String* string){
    assert(string_is_sane(string));
    if(!string->length){ return 0; }
    for(int i = 0; i < string->length; i++){
        if(ch == string->body[i]){
            assert(string_is_sane(string));
            return 1;
        }
    }
    assert(string_is_sane(string));
    return 0;
}

struct String string_copy_stack(struct String *from) {
    assert(string_is_sane(from));
    struct String s = {
        .length = 0,
        .body = NULL
    };
    string_copy_to(&s, from);
    assert(string_is_sane(&s));
    return s;
}

struct String *copy_string_heap(struct String *from) {
    assert(string_is_sane(from));
    struct String *s = malloc(sizeof(struct String));
    s->length = 0;
    string_copy_to(s, from);
    assert(string_is_sane(s));
    return s;
}

void string_copy_to(struct String *to, struct String *from) {
    assert(string_is_sane(from));
    assert(string_is_sane(to));
    if (from->length == 0) {
        RESIZE(to->body, 0);
        to->length = 0;
    } else {
        if (to->length != from->length) {
            RESIZE(to->body, from->length + 1); // 0 byte at end
            strcpy(to->body, from->body);
            to->length = from->length;
        }
    }
    assert(string_is_sane(from));
    assert(string_is_sane(to));
}

int string_matches(struct String *base, struct String *compare){
    assert(string_is_sane(base));
    assert(string_is_sane(compare));
    if(base->length != compare->length){
        return 0;
    }
    int counter = 0;
    while(counter < base->length){
        if(base->body[counter] != compare->body[counter]){
            return 0;
        }
        counter++;
    }
    return 1;
}