#include <stdio.h>
#include <stdlib.h>
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "constants.h"
#include "execute.h"
#include "execution_functions/for_each.h"
#include "execution_functions/for_loop.h"
#include "execution_functions/reduce.h"
#include "execution_functions/read_file.h"
#include "execution_functions/map_array.h"
#include "execution_functions/let_binding.h"
#include "execution_functions/reduce_ast.h"
#include "execution_functions/if_block.h"
#include "execution_functions/switch.h"
#include "execution_functions/parallel_execution.h"
#include "execution_functions/while_loop.h"
#include "execution_functions/read_line.h"
#include "execution_functions/evaluate.h"
#include "apply_core_function.h"

struct Value execute (struct Tree * ast, hash_table *defined, struct Scopes * scopes, int max_depth){
    struct Value result;
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    // first check for special kinds of execution
    if(ast->type == 'k'  && ast->content.type == KEYWORD && string_matches(&ast->content.data.str, &if_const)){
        result = if_block(ast, defined, scopes, max_depth-1);
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&let_const, &ast->content.data.str)){
        if (ast->size < 2) {
            ERROR("Not enough arguments for store_let_binding: %i < 2\n", ast->size);
        }
        store_let_binding(ast->children[0],ast->children[1], defined, scopes, max_depth-1);
        result.type = UNDEF;
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&each_const, &ast->content.data.str)){
        for_each(ast, defined, scopes, max_depth-1);
        result.type = UNDEF;
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&map_const, &ast->content.data.str)){
        result = map_array(ast, defined, scopes, max_depth-1);
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&while_const, &ast->content.data.str)){
        while_loop(ast, defined, scopes, max_depth - 1);
        result.type = UNDEF;
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&reduce_const, &ast->content.data.str)){
        result = reduce_array(ast, defined, scopes, max_depth - 1);
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&set_const, &ast->content.data.str)) {
        if (ast->size < 3) {
            ERROR("Not enough arguments for set: %i < 3", ast->size);
        }
        struct Value index = execute(ast->children[0], defined, scopes, max_depth-1);
        struct Value item = execute(ast->children[1], defined, scopes, max_depth-1);
        struct Value array = execute(ast->children[2], defined, scopes, max_depth-1);
        if(array.type == STRING){
            result = string_set(index, item, array);
        }
        else if(array.type == ARRAY){
            result = array_set(index, item, array);
        } else {
            ERROR("Error using set with wrong type ('%c' !== ['a'|'s'])", array.type);
        }
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&for_const, &ast->content.data.str)){

        for_loop(ast, defined, scopes, max_depth - 1);
        result.type = UNDEF; //result = undefined
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&do_const, &ast->content.data.str)){
        for(int i = 0; i < ast->size; i++){
            if(i == ast->size-1){
                result = execute(ast->children[i], defined, scopes, max_depth-1);
            } else {
                execute(ast->children[i], defined, scopes, max_depth-1);
            }
        }
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&read_const, &ast->content.data.str)){
        if (ast->size < 1) {
            ERROR("Not enough arguments for read: %i < 1", ast->size);
        }
        char t = ast->children[0]->content.type;
        if (t != STRING && t != KEYWORD) {
            ERROR("Invalid type for read: '%c' != [STRING, KEYWORD]", t);
        }
        result = read_file(ast->children[0]->content.data.str);
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&substring_const, &ast->content.data.str)){
        if (ast->size < 3) {
            ERROR("Not enough arguments for substring: %i < 3", ast->size);
        }
        struct Value string = execute(ast->children[2], defined, scopes, max_depth-1);
        struct Value start = execute(ast->children[0], defined, scopes, max_depth-1);
        struct Value end = execute(ast->children[1], defined, scopes, max_depth-1);
        if(string.type != STRING){
            ERROR("Non-string value passed into substring: %c.", string.type);
            result.type = UNDEF;
            ;
        } else {
            result = substring(start.data.ln, end.data.ln, string);
        }
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&switch_const, &ast->content.data.str)){
        result = switch_case(ast, defined, scopes, max_depth-1);
    }
    else if(ast->type == 'k' && ast->content.type == KEYWORD && string_matches(&parallel_const, &ast->content.data.str)){
        parallel_execution(ast, defined, scopes, max_depth-1);
        result.type = UNDEF;
    } else {
        // no special execution types found, check for more basic conditions
        struct Tree *func;
        if(!ast->size){
            // ast with no children is either a value or a variable
            result = value_copy_stack(&ast->content);
            sub_vars(&result, scopes, max_depth - 1);
        }
        else if(ast->type == 'k' && ast->content.type == KEYWORD && (func = get_defined_func(defined, ast->content.data.str)) != NULL){
            make_args_map(ast, defined, scopes, func, max_depth-1);
            result = execute(duplicate_tree(get_defined_body(func)), defined, scopes, max_depth-1);
            scopes->current--;
        }
        else if(ast->size == 1){
            struct Value a = execute(ast->children[0], defined, scopes, max_depth - 1);
            if(ast->type == 'k' && ast->content.type == KEYWORD){
                if(string_matches(&ast->content.data.str, &print_const)){
                    print(a);
                    printf("\n");
                    result.type = UNDEF;
                }
                else if(string_matches(&ast->content.data.str, &length_const)){
                    result = value_from_long(length(a));
                }
                else if(string_matches(&ast->content.data.str, &return_const)){
                    result = a;
                }
                else if(string_matches(&ast->content.data.str, &eval_const)){
                    result = eval(&a, defined, scopes, max_depth - 1);
                }
                else if(string_matches(&ast->content.data.str, &read_line_const)){
                    result = read_line(&a);
                }
            }
        }
        else if(ast->size == 2) {
            struct Value a = execute(ast->children[0], defined, scopes, max_depth-1);
            struct Value b = execute(ast->children[1], defined, scopes, max_depth-1);
            result = apply_core_function(ast, a, b);
        } else {
            result = reduce_ast(ast, defined, scopes, max_depth-1);
        }
    }
    return result;
}
