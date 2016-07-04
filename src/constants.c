#include "constants.h"
#include "base_util.h"
#include "config.h"

const int RECURSE_MAX = 250;
const int LOOP_MAX = 10000;

struct String functions = STR_NEW("*+-/!=<>");

struct String numbers = STR_NEW("0123456789");

struct String letters = STR_NEW("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

struct String print_const = STR_NEW("print");

struct String concat_const = STR_NEW("cat");

struct String if_const = STR_NEW("if");

struct String def_const = STR_NEW("def");

struct String let_const = STR_NEW("let");

struct String length_const = STR_NEW("length");

struct String index_const = STR_NEW("index");

struct String push_const = STR_NEW("push");

struct String each_const = STR_NEW("each");

struct String map_const = STR_NEW("map");

struct String reduce_const = STR_NEW("reduce");

struct String set_const = STR_NEW("set");

struct String for_const = STR_NEW("for");

struct String do_const = STR_NEW("do");

struct String read_const = STR_NEW("read");

struct String import_const = STR_NEW("import");

struct String substring_const = STR_NEW("substring");

struct String split_const = STR_NEW("split");

struct String return_const = STR_NEW("return");

struct String parallel_const = STR_NEW("parallel");

struct String switch_const = STR_NEW("switch");

struct String default_const = STR_NEW("default");

struct String while_const = STR_NEW("while");

struct String eval_const = STR_NEW("eval");

struct String read_line_const = STR_NEW("readline");

const char open_parens_const = '(';
const char close_parens_const = ')';
const char root_type_const = 'r';
const char quote_const = '"';
const char bracket_const = '[';
const char end_bracket_const = ']';
const char dot_const = '.';

int is_a_open_parens(char ch){ return ch == open_parens_const; }
int is_a_close_parens(char ch){ return ch == close_parens_const; }
int is_a_root_type(char ch){ return ch == root_type_const; }
int is_a_quote(char ch){ return ch == quote_const; }
int is_a_bracket(char ch){ return ch == bracket_const; }
int is_a_end_bracket(char ch){ return ch == end_bracket_const; }

int is_a_letter(char ch){
    return string_contains(ch, &letters);
}

int is_a_number(char ch){
    return string_contains(ch, &numbers);
}

int is_a_function(char ch){
  return string_contains(ch, &functions);
}
