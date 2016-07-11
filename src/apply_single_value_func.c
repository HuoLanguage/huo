#include <stdio.h>
#include <stdlib.h>
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "constants.h"
#include "execute.h"
#include "execution_functions/read_line.h"
#include "execution_functions/evaluate.h"

struct Value apply_single_value_func(struct Execution_bundle * exec_bundle){
    struct Tree * ast = exec_bundle->ast;
    exec_bundle->ast = ast->children[0];
    struct Value value = execute(exec_bundle);

    if(string_matches_heap(&ast->content.data.str, &print_const)){
        print(value);
        printf("\n");
        value.type = UNDEF;
        value.data.bl = false;
    } else if(string_matches_heap(&ast->content.data.str, &length_const)){
        value = value_from_long(length(value));
    } else if(string_matches_heap(&ast->content.data.str, &return_const)){
        return value;
    } else if(string_matches_heap(&ast->content.data.str, &eval_const)){
        return eval(&value, exec_bundle);
    } else if(string_matches_heap(&ast->content.data.str, &read_line_const)){
        return value_from_string(read_line(&value));
    } else {
        value.type = UNDEF;
        value.data.bl = false;
    }
    return value;
}
