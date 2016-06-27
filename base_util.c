#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "structures.h"
#include "base_util.h"
#include "core_functions.h"

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

struct Value copy_value_stack(struct Value * b){
    struct Value a;
    a.type = b->type;
    if(b->type == 's' || b->type == 'k'){
        assert(string_is_sane(&b->data.str));
        a.data.str = string_copy_stack(&b->data.str);
        assert(string_is_sane(&a.data.str));
    } else if(b->type == 'f'){
        a.data.fl = b->data.fl;
    } else if(b->type == 'l'){
        a.data.ln = b->data.ln;
    } else if (b->type == 'b'){
        a.data.bl = b->data.bl;
    } else if (b->type == 'a'){
        copy_array(&a, b->data.array);
    } else if (b->type == 'u'){
        //a.type == 'u';
    } else {
        ERROR("Unknown type: %c", a.type);
    }
    return a;
}

struct Value *copy_value_heap(struct Value * b){
    struct Value *a = malloc(sizeof(struct Value));
    if (a == NULL) {
        ERROR("Malloc failure");
    }
    a->type = b->type;
    if(b->type == 's' || b->type == 'k'){
        assert(string_is_sane(&b->data.str));
        a->data.str = string_copy_stack(&b->data.str);
        assert(string_is_sane(&a->data.str));
    } else if(b->type == 'f'){
        a->data.fl = b->data.fl;
    } else if(b->type == 'l'){
        a->data.ln = b->data.ln;
    } else if (b->type == 'b'){
        a->data.bl = b->data.bl;
    } else if (b->type == 'a'){
        copy_array(a, b->data.array);
    } else if (b->type == 'u'){
        //a->type == 'u';
    } else {
        ERROR("Unknown type: %c", a->type);
    }
    return a;
}

void copy_value_to(struct Value * a, struct Value * b){
    a->type = b->type;
    if(a->type == 's' || a->type == 'k'){
        assert(string_is_sane(&a->data.str));
        assert(string_is_sane(&b->data.str));
        string_copy_to(&a->data.str, &b->data.str);
        assert(string_is_sane(&a->data.str));
        assert(string_is_sane(&b->data.str));
    } else if(a->type == 'f'){
        a->data.fl = b->data.fl;
    } else if(a->type == 'l'){
        a->data.ln = b->data.ln;
    } else if (a->type == 'b'){
        a->data.bl = b->data.bl;
    } else if (a->type == 'a'){
        copy_array(a, b->data.array);
    } else if (b->type == 'u'){
       // a->type == 'u';
    } else {
        ERROR("Unknown type: %c", a->type);
    }
}

void copy_array(struct Value * a, struct Value_array * b){
    struct Value_array * array = malloc(sizeof(struct Value_array));
    array->size = b->size;
    for(int i = 0; i < b->size; i++){
        array->values[i] = copy_value_heap(b->values[i]);
    }
    a->data.array = array;
}

struct Tree * duplicate_tree(struct Tree * a){
    struct Tree * root = malloc(sizeof(struct Tree));
    root->type = a->type;
    root->size = 0;
    root->content = copy_value_stack(&a->content);
    for(int i = 0; i < a->size; i++){
        root->children[i] = duplicate_tree(a->children[i]);
        root->size++;
    }
    return root;
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

void make_scope(struct Scopes * scopes){
    scopes->scopes[scopes->size] = malloc(sizeof(struct Map));
    scopes->scopes[scopes->size]->size = 0;
    scopes->size++;
    scopes->current++;
}

void printTree(struct Tree *tree){
    if(!tree->size){
      if(tree->type == 'k'){
          printf("{ type: \'%c\', size: %d, keyword: %s }", tree->type, tree->size, tree->content.data.str.body);
      }
      else if(tree->type == 'n'){
          printf("{ type: \'%c\', size: %d, value: %ld }", tree->type, tree->size, tree->content.data.ln);
      }
      else {
          printf("{ type: \'%c\', size: %d }", tree->type, tree->size);
      }
    } else {
      if(tree->type == 'k'){
          printf("{ type: \'%c\', size: %d, keyword: %s ", tree->type, tree->size, tree->content.data.str.body);
      }
      else if(tree->type == 'n'){
          printf("{ type: \'%c\', size: %d, value: %ld ", tree->type, tree->size, tree->content.data.ln);
      } else {
          printf("{ type: \'%c\', size: %d, ", tree->type, tree->size);
      }
      if(tree->size && tree->size < 100){
        printf("children: [");
        int counter = 0;
        while(counter < tree->size){
          printTree(tree->children[counter]);
          counter++;
        }
      }
      printf("]}");
    }
}
