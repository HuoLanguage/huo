#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../base_util.h"
#include <stdbool.h>
#include "string.h"
#include "../config.h"
#include "value.h"


bool string_is_sane(const struct String *const s) {
    assert (s != NULL);
    assert(s->length == 0 || s->body != NULL);
    assert(s->length == 0 || s->body[s->length] == 0);
    return true;
}

struct String string_from_char(char c) {
    struct String s = {
        .length = 1,
        .body = ARR_MALLOC(2, char)
    };
    s.body[0] = c;
    s.body[1] = 0;
    assert(string_is_sane(&s));
    return s;
}

struct String string_from_chars(const char *str) {
    struct String s;
    if (str == NULL) {
        s.length = 0;
        s.body = NULL;
    } else {
        int l = strlen(str);
        s.length = l;
        s.body = ARR_MALLOC(l + 1, char);
        strcpy(s.body, str);
        s.body[s.length] = 0;
    }
    assert(string_is_sane(&s));
    return s;
}

struct String string_concat_stack(struct String *a, struct String *b) {
    struct String temp = string_copy_stack(a);
    string_concat_to(&temp, b);
    return temp;
}

struct String *string_concat_heap(struct String *a, struct String *b) {
    struct String *temp = string_copy_heap(a);
    string_concat_to(temp, b);
    return temp;
}

void string_concat_to(struct String *to, struct String *from) {
    assert(string_is_sane(from));
    assert(string_is_sane(to));
    int len = to->length;
    to->length += from->length;
    RESIZE(to->body, to->length + 1);
    for(size_t l = 0; l < from->length; l++){
        to->body[l+len] = from->body[l];
    }
    to->body[to->length] = 0;
    assert(string_is_sane(from));
    assert(string_is_sane(to));
}

void string_concat_to_consume(struct String *to, struct String s) {
    string_concat_to(to, &s);
    string_free(&s);
}

void string_concat_to_chars(struct String *to, char *from) {
    assert(string_is_sane(to));
    int len = to->length;
    size_t from_length = strlen(from);
    to->length += from_length;
    RESIZE(to->body, to->length + 1);
    for(size_t l = 0; l < from_length; l++){
        to->body[l+len] = from[l];
    }
    to->body[to->length] = 0;
    assert(string_is_sane(to));
}
void string_concat_to_long(struct String *to, huo_int_t from) {
    assert(string_is_sane(to));

    size_t int_size = snprintf(NULL, 0, "%"PRIhi, from);

    int len = to->length;
    to->length += int_size;
    RESIZE(to->body, to->length + 1);
    sprintf(&to->body[len], "%"PRIhi, from);
    assert(string_is_sane(to));
}
void string_concat_to_float(struct String *to, float from) {
    assert(string_is_sane(to));

    size_t int_size = snprintf(NULL, 0, "%f", from);

    int len = to->length;
    to->length += int_size;
    RESIZE(to->body, to->length + 1);
    sprintf(&to->body[len], "%f", from);
    assert(string_is_sane(to));
}

bool string_contains(char ch, const struct String* string){
    assert(string_is_sane(string));
    if(!string->length){ return false; }
    for(size_t i = 0; i < string->length; i++){
        if(ch == string->body[i]){
            assert(string_is_sane(string));
            return true;
        }
    }
    assert(string_is_sane(string));
    return false;
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

struct String *string_copy_heap(struct String *from) {
    assert(string_is_sane(from));
    struct String *s = malloc_or_die(sizeof(struct String));
    s->length = 0;
    s->body = NULL;
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
            to->length = from->length;
        }
        strcpy(to->body, from->body);
    }
    assert(string_is_sane(from));
    assert(string_is_sane(to));
}

bool string_matches_stack(struct String base, struct String compare) {
    return string_matches_heap(&base, &compare);
}

bool string_matches_heap(struct String *base, struct String *compare){
    assert(string_is_sane(base));
    assert(string_is_sane(compare));
    if(base->length != compare->length){
        return false;
    }
    size_t counter = 0;
    while(counter < base->length){
        if(base->body[counter] != compare->body[counter]){
            return false;
        }
        counter++;
    }
    return true;
}
bool string_matches_char(struct String *base, char c) {
    assert(string_is_sane(base));
    return (base->length == 1 && base->body[0] == c);
}

char *string_to_chars(const struct String *s) {
    assert(string_is_sane(s));
    if (s->length == 0) {
        return "";
    }
    return s->body;
}
size_t string_length(struct String *s) {
    assert(string_is_sane(s));
    return s->length;
}
char string_index(struct String *s, size_t i) {
    assert(string_is_sane(s));
    if (i >= s->length) {
        ERROR("Index out of range: %zu >= %zu", i, s->length);
    }
    return s->body[i];
}

unsigned long string_hash_code(struct String *s) {
    // djb2
    unsigned long hash = 5381;
    if (s->length == 0)
        return hash;

    char *pos = s->body;

    while (*pos != 0)
        hash = hash * 33 + *(pos++);

    return hash;
}
unsigned long string_hash_code_vv(void *s) {
    return string_hash_code((struct String *) s);
}

bool string_matches_vv(void *base, void *compare) {
    return string_matches_heap((struct String *) base, (struct String *) compare);
}

void string_free_stack(struct String s) {
    if (s.body != NULL) {
        free(s.body);
        s.body = NULL;
    }
    s.length = 0;
}

void string_free(struct String *const s) {
    if (s != NULL) {
        string_free_stack(*s);
        free(s);
    }
}
