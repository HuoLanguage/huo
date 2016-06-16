#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <stddef.h>
#include "structures.h"
#include "base_util.h"

extern struct String functions;
extern struct String numbers;
extern struct String letters;
extern struct String print_const;
extern struct String concat_const;
extern struct String if_const;
extern struct String def_const;
extern struct String let_const;
extern struct String length_const;
extern struct String index_const;
extern struct String push_const;
extern struct String each_const;
extern struct String map_const;
extern struct String reduce_const;
extern struct String set_const;
extern struct String for_const;
extern struct String do_const;

extern const char bool_true;
extern const char bool_false;
extern const char bool_undefined;
extern const char open_parens_const;
extern const char close_parens_const;
extern const char root_type_const;
extern const char quote_const;
extern const char bracket_const;
extern const char end_bracket_const;
extern const char dot_const;

int is_a_open_parens(char ch);
int is_a_close_parens(char ch);
int is_a_root_type(char ch);
int is_a_quote(char ch);
int is_a_letter(char ch);
int is_a_number(char ch);
int is_a_function(char ch);
int is_a_bracket(char ch);
int is_a_end_bracket(char ch);

#endif
