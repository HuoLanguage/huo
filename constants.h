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

struct String functions = {
    .body = "*+-_/\0",
    .length = 5
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
