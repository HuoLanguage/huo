#include <stdio.h>
#include <stdlib.h>
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "constants.h"
#include "execute.h"
#include "execution_functions/reduce_ast.h"
#include "apply_core_function.h"
#include "apply_execution_function.h"
#include "apply_single_value_func.h"
#include "config.h"

struct Value execute (struct Execution_bundle * exec_bundle){
    exec_bundle->max_depth -= 1;

    struct Tree * ast = exec_bundle->ast;
    struct Scopes * scopes = exec_bundle->scopes;
    hash_table * defined = exec_bundle->defined;
    huo_depth_t max_depth = exec_bundle->max_depth;
    struct Value_array * function_names = exec_bundle->function_names;

    struct Value result;
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    // first check for special kinds of execution
    if(ast->content.type == KEYWORD && array_contains(&ast->content, function_names)){
        result = apply_execution_function(exec_bundle);
    } else {
        // no special execution types found, check for more basic conditions
        struct Tree *func;
        if(ast->content.type == KEYWORD && (func = get_defined_func(defined, ast->content.data.str)) != NULL){
            make_args_map(exec_bundle, func);
            exec_bundle->ast = duplicate_tree(get_defined_body(func));
            result = execute(exec_bundle);
            scopes->current--;
        }
        else if(!ast->size){
            // functions are all checked above, if we are not a function, then
            // an ast with no children is either a value or a variable
            result = value_copy_stack(&ast->content);
            if(ast->type == 'k' && ast->content.type == KEYWORD){
                if(string_matches_heap(&ast->content.data.str, &false_const)){
                    result = value_from_bool(false);
                } else if(string_matches_heap(&ast->content.data.str, &true_const)){
                    result = value_from_bool(true);
                }
            }
            sub_vars(&result, scopes, max_depth - 1);
        }
        else if(ast->size == 1){
            if(ast->type == 'k' && ast->content.type == KEYWORD){
                result = apply_single_value_func(exec_bundle);
            }
        }
        else if(ast->size == 2) {
            exec_bundle->ast = ast->children[0];
            struct Value a = execute(exec_bundle);

            exec_bundle->ast = ast->children[1];
            struct Value b = execute(exec_bundle);

            result = apply_core_function(ast, a, b);
        } else {
            result = reduce_ast(exec_bundle);
        }
    }

    exec_bundle->max_depth += 1;
    return result;
}
