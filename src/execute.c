#include <stdio.h>
#include <stdlib.h>
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "constants.h"
#include "execute.h"
#include "execution_functions/reduce_ast.h"
#include "execution_functions/read_line.h"
#include "execution_functions/evaluate.h"
#include "apply_core_function.h"
#include "apply_execution_function.h"
#include "config.h"

struct Value execute (struct Tree *ast, hash_table *defined, struct Scopes *scopes, struct Value_array *function_names, huo_depth_t max_depth){
    struct Value result;
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    // first check for special kinds of execution
    if(ast->content.type == KEYWORD && array_contains(&ast->content, function_names)){
        result = apply_execution_function(ast, defined, scopes, function_names, max_depth);
    } else {
        // no special execution types found, check for more basic conditions
        struct Tree *func;
        if(!ast->size){
            // ast with no children is either a value or a variable
            result = value_copy_stack(&ast->content);
            sub_vars(&result, scopes, max_depth - 1);
        }
        else if(ast->content.type == KEYWORD && (func = get_defined_func(defined, ast->content.data.str)) != NULL){
            make_args_map(ast, defined, scopes, function_names, func, max_depth-1);
            result = execute(duplicate_tree(get_defined_body(func)), defined, scopes, function_names, max_depth-1);
            scopes->current--;
        }
        else if(ast->size == 1){
            struct Value a = execute(ast->children[0], defined, scopes, function_names, max_depth-1);
            if(ast->type == 'k' && ast->content.type == KEYWORD){
                if(string_matches_heap(&ast->content.data.str, &print_const)){
                    print(a);
                    printf("\n");
                    result.type = UNDEF;
                }
                else if(string_matches_heap(&ast->content.data.str, &length_const)){
                    result = value_from_long(length(a));
                }
                else if(string_matches_heap(&ast->content.data.str, &return_const)){
                    result = a;
                }
                else if(string_matches_heap(&ast->content.data.str, &eval_const)){
                    result = eval(&a, defined, scopes, function_names, max_depth - 1);
                }
                else if(string_matches_heap(&ast->content.data.str, &read_line_const)){
                    result = value_from_string(read_line(&a));
                }
            }
        }
        else if(ast->size == 2) {
            struct Value a = execute(ast->children[0], defined, scopes, function_names, max_depth-1);
            struct Value b = execute(ast->children[1], defined, scopes, function_names, max_depth-1);
            result = apply_core_function(ast, a, b);
        } else {
            result = reduce_ast(ast, defined, scopes, function_names, max_depth-1);
        }
    }
    return result;
}
