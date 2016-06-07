#include "constants.h"

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

struct String let_const = {
    .body = "let",
    .length = 3
};

const char bool_true = 't';
const char bool_false = 'f';
const char bool_undefined = 'u';

const char open_parens_const = '(';
const char close_parens_const = ')';
const char root_type_const = 'r';
const char quote_const = '"';

int is_a_open_parens(char ch){ return ch == open_parens_const; }
int is_a_close_parens(char ch){ return ch == close_parens_const; }
int is_a_root_type(char ch){ return ch == root_type_const; }
int is_a_quote(char ch){ return ch == quote_const; }

int is_a_letter(char ch){
    return string_contains(ch, &letters);
}

int is_a_number(char ch){
    return string_contains(ch, &numbers);
}

int is_a_function(char ch){
  return string_contains(ch, &functions);
}
