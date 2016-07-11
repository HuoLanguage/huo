#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <stddef.h>
#include "structures/string.h"
#include "base_util.h"


extern const huo_depth_t RECURSE_MAX;
extern const huo_int_t LOOP_MAX;

extern struct String functions;
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
extern struct String read_file_const;
extern struct String import_const;
extern struct String substring_const;
extern struct String split_const;
extern struct String return_const;
extern struct String parallel_const;
extern struct String switch_const;
extern struct String default_const;
extern struct String while_const;
extern struct String eval_const;
extern struct String read_line_const;
extern struct String function_names;

extern const char open_parens_const;
extern const char close_parens_const;
extern const char root_type_const;
extern const char quote_const;
extern const char bracket_const;
extern const char end_bracket_const;
extern const char dot_const;

bool is_a_open_parens(char ch);
bool is_a_close_parens(char ch);
bool is_a_root_type(char ch);
bool is_a_quote(char ch);
bool is_a_function(char ch);
bool is_a_bracket(char ch);
bool is_a_end_bracket(char ch);

#endif
