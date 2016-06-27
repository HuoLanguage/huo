#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
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
#include "apply_core_function.h"

struct Value execute (struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes, int max_depth){
    struct Value result;
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    // first check for special kinds of execution
    if(ast->type == 'k' && string_matches(&if_const, &ast->content.data.str)){
        result = if_block(ast, defined, scopes, max_depth-1);
    }
    else if(ast->type == 'k' && string_matches(&let_const, &ast->content.data.str)){
        if (ast->size < 2) {
            ERROR("Not enough arguments for store_let_binding: %i < 2\n", ast->size);
        }
        store_let_binding(ast->children[0],ast->children[1], defined, scopes, max_depth-1);
        result.type = 'u';
    }
    else if(ast->type == 'k' && string_matches(&each_const, &ast->content.data.str)){
        for_each(ast, defined, scopes, max_depth-1);
        result.type = 'u';
    }
    else if(ast->type == 'k' && string_matches(&map_const, &ast->content.data.str)){
        result = map_array(ast, defined, scopes, max_depth-1);
    }
    else if(ast->type == 'k' && string_matches(&while_const, &ast->content.data.str)){
        while_loop(ast, defined, scopes, max_depth - 1);
        result.type = 'u';
    }
    else if(ast->type == 'k' && string_matches(&reduce_const, &ast->content.data.str)){
        result = reduce_array(ast, defined, scopes, max_depth-1);
    }
    else if(ast->type == 'k' && string_matches(&set_const, &ast->content.data.str)){
        if (ast->size < 3) {
            ERROR("Not enough arguments for set: %i < 3", ast->size);
        }
        struct Value index = execute(ast->children[0], defined, scopes, max_depth-1);
        struct Value item = execute(ast->children[1], defined, scopes, max_depth-1);
        struct Value array = execute(ast->children[2], defined, scopes, max_depth-1);
        if(array.type == 's'){
            result = string_set(index, item, array);
        }
        else if(array.type == 'a'){
            result = array_set(index, item, array);
        } else {
            ERROR("Error using set with wrong type ('%c' !== ['a'|'s'])", array.type);
        }
        ;
    }
    else if(ast->type == 'k' && string_matches(&for_const, &ast->content.data.str)){
        for_loop(ast, defined, scopes, max_depth - 1);
        result.type = 'u'; //result = undefined
    }
    else if(ast->type == 'k' && ast->content.type == 'k' && string_matches(&do_const, &ast->content.data.str)){
        for(int i = 0; i < ast->size; i++){
            if(i == ast->size-1){
                result = execute(ast->children[i], defined, scopes, max_depth-1);
            } else {
                execute(ast->children[i], defined, scopes, max_depth-1);
            }
        }
    }
    else if(ast->type == 'k' && string_matches(&read_const, &ast->content.data.str)){
        if (ast->size < 1) {
            ERROR("Not enough arguments for read: %i < 1", ast->size);
        }
        char t = ast->children[0]->content.type;
        if (t != 's' && t != 'k') {
            ERROR("Invalid type for read: '%c' != ['s', 'k']", t);
        }
        result = read_file(ast->children[0]->content.data.str);
    }
    else if(ast->type == 'k' && string_matches(&substring_const, &ast->content.data.str)){
        if (ast->size < 3) {
            ERROR("Not enough arguments for substring: %i < 3", ast->size);
        }
        struct Value string = execute(ast->children[2], defined, scopes, max_depth-1);
        struct Value start = execute(ast->children[0], defined, scopes, max_depth-1);
        struct Value end = execute(ast->children[1], defined, scopes, max_depth-1);
        if(string.type != 's'){
            ERROR("Non-string value passed into substring: %c.", string.type);
            result.type = 'u';
            ;
        } else {
            result = substring(start.data.ln, end.data.ln, string);
        }
    }
    else if(ast->type == 'k' && string_matches(&switch_const, &ast->content.data.str)){
        result = switch_case(ast, defined, scopes, max_depth-1);
    }
    else if(ast->type == 'k' && string_matches(&parallel_const, &ast->content.data.str)){
        parallel_execution(ast, defined, scopes, max_depth-1);
        result.type = 'u';
    } else {
        // no special execution types found, check for more basic conditions
        int idx;
        if(!ast->size){
            // ast with no children is either a value or a variable
            if(ast->type == 'k'){
                int found = 0;
                struct Map * current_scope = scopes->scopes[scopes->current];
                for(int i = 0; i < current_scope->size; i++){
                    if(string_matches(&current_scope->members[i]->key->data.str, &ast->content.data.str)){
                        result = *current_scope->members[i]->val;
                        found = 1;
                    }
                }
                if(!found){
                    ERROR("Undefined variable: %s", ast->content.data.str.body);
                }
            } else {
                result = ast->content;
            }
        }
        else if(ast->type == 'k' && (idx = is_defined_func(defined, ast->content.data.str)) > -1){
            make_args_map(ast, defined, scopes, idx, max_depth-1);
            result = execute(duplicate_tree(get_defined_body(defined->trees[idx])), defined, scopes, max_depth-1);
            scopes->current--;
        }
        else if(ast->size == 1){
            struct Value a = execute(ast->children[0], defined, scopes, max_depth - 1);
            if(ast->type == 'k'){
                if(string_matches(&ast->content.data.str, &print_const)){
                    print(a);
                    printf("\n");
                    result.type = 'u';
                }
                else if(string_matches(&ast->content.data.str, &length_const)){
                    result = length(a);
                }
                else if(string_matches(&ast->content.data.str, &return_const)){
                    result = execute(ast->children[0], defined, scopes, max_depth-1);
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
