#include <stddef.h>
#include "structures.h"

#define CHAR_ISA(name, constant) \
int is_a_##name(char ch){ return ch == constant; }

#define CHAR_ISA_TYPE(name, type) \
int is_a_##name(char ch){ \
    return string_contains(ch, &type); \
}

int string_contains(char ch, struct String* string){
    if(!string->length){ return 0; }
    for(int i = 0; i < string->length; i++){
        if(ch == string->body[i]){
            return 1;
        }
    }
    return 0;
}

void copy_value(struct Value * a, struct Value * b){
    a->type = b->type;
    if(a->type == 's'){
        a->data.str.length = b->data.str.length;
        strcpy(a->data.str.body, b->data.str.body);
    } else if(a->type == 'f'){
        a->data.fl = b->data.fl;
    } else if(a->type == 'l'){
        a->data.ln = b->data.ln;
    } else if (a->type == 'b'){
        a->data.bl = b->data.bl;
    }
}

int string_matches(struct String base, struct String compare){
    if(base.length != compare.length){
        return 0;
    }
    int counter = 0;
    while(counter < base.length){
        if(base.body[counter] != compare.body[counter]){
            return 0;
        }
        counter++;
    }
    return 1;
}

void printTree(struct Tree *tree){
    if(!tree->size){
      printf("{ type: \'%c\', size: %d }", tree->type, tree->size);
    } else {
      printf("{ type: \'%c\', size: %d, ", tree->type, tree->size);
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
};

struct String functions = {
    .body = "*+-_/!=\0",
    .length = 7
};

struct String numbers = {
    .body = "0123456789\0",
    .length = 10
};

struct String letters = {
    .body = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
    .length = 52
};

struct String print_const = {
    .body = "print",
    .length = 5
};

struct String concat_const = {
    .body = "cat",
    .length = 3
};

struct String if_const = {
    .body = "if",
    .length = 2
};

struct String def_const = {
    .body = "def",
    .length = 3
};

const char bool_true = 't';
const char bool_false = 'f';
const char bool_undefined = 'u';

const char open_parens_const = '(';
const char close_parens_const = ')';
const char root_type_const = 'r';
const char quote_const = '"';

CHAR_ISA(open_parens, open_parens_const);
CHAR_ISA(close_parens, close_parens_const);
CHAR_ISA(root_type, root_type_const);
CHAR_ISA(quote, quote_const);
CHAR_ISA_TYPE(letter, letters);
CHAR_ISA_TYPE(number, numbers);
CHAR_ISA_TYPE(function, functions);
