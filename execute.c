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
#include "execution_functions/parallel_execution.h"
#include "apply_core_function.h"

struct Value execute (struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth){
    struct Value result;
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    // first check for special kinds of execution
    if(ast->type == 'k' && string_matches(&ast->content.data.str, &if_const)){
        return if_block(ast, defined, let_map, max_depth - 1);
    }
    else if(ast->type == 'k' && string_matches(&let_const, &ast->content.data.str)){
        store_let_binding(ast, defined, let_map, max_depth - 1);
        result.type = 'u';
    }
    else if(ast->type == 'k' && string_matches(&each_const, &ast->content.data.str)){
        for_each(ast, defined, let_map, max_depth - 1);
        result.type = 'u';
    }
    else if(ast->type == 'k' && string_matches(&map_const, &ast->content.data.str)){
        return map_array(ast, defined, let_map, max_depth - 1);
    }
    else if(ast->type == 'k' && string_matches(&reduce_const, &ast->content.data.str)){
        return reduce_array(ast, defined, let_map, max_depth - 1);
    }
    else if(ast->type == 'k' && string_matches(&set_const, &ast->content.data.str)){
        if (ast->size < 3) {
            ERROR("Not enough arguments for set: %i < 3", ast->size);
        }
        struct Value index = execute(ast->children[0], defined, let_map, max_depth-1);
        struct Value item = execute(ast->children[1], defined, let_map, max_depth-1);
        struct Value array = execute(ast->children[2], defined, let_map, max_depth-1);
        result = array_set(index, item, array);
        return result;
    }
    else if(ast->type == 'k' && string_matches(&for_const, &ast->content.data.str)){
        for_loop(ast, defined, let_map, max_depth - 1);
        result.type = 'u'; //return undefined
    }
    else if(ast->type == 'k' && string_matches(&do_const, &ast->content.data.str)){
        for(int i = 0; i < ast->size; i++){
            if(i == ast->size-1){
                result = execute(ast->children[i], defined, let_map, max_depth - 1);
            } else {
                execute(ast->children[i], defined, let_map, max_depth - 1);
            }
        }
    }
    else if(ast->type == 'k' && string_matches(&read_const, &ast->content.data.str)){
        if (ast->size < 1) {
            ERROR("Not enough arguments for read: %i < 1", ast->size);
        }
        return read_file(ast->children[0]->content.data.str);
    }
    else if(ast->type == 'k' && string_matches(&substring_const, &ast->content.data.str)){
        if (ast->size < 3) {
            ERROR("Not enough arguments for substring: %i < 3", ast->size);
        }
        struct Value string = execute(ast->children[2], defined, let_map, max_depth-1);
        struct Value start = execute(ast->children[0], defined, let_map, max_depth-1);
        struct Value end = execute(ast->children[1], defined, let_map, max_depth-1);
        if(string.type != 's'){
            ERROR("Non-string value passed into substring: %c.", string.type);
            result.type = 'u';
            return result;
        } else {
            return substring(start.data.ln, end.data.ln, string);
        }
    }
    else if(ast->type == 'k' && string_matches(&parallel_const, &ast->content.data.str)){
        parallel_execution(ast, defined, let_map, max_depth - 1);
        result.type = 'u';
    } else {
        // no special execution types found, check for more basic conditions
        int idx;
        if(!ast->size){
            // ast with no children is either a value or a variable
            if(ast->type == 'k'){
                for(int i = 0; i < let_map->size; i++){
                    if(string_matches(&let_map->members[i]->key->data.str, &ast->content.data.str)){
                        return *let_map->members[i]->val;
                    }
                }
                ERROR("Undefined variable: %s", ast->content.data.str.body);
            } else if (ast->type == 's' || ast->type == 'n') {
                return ast->content;
            } else {
                ERROR("Cannot get value of type '%c'", ast->type);
            }
        }
        else if(ast->type == 'k' && (idx = is_defined_func(defined, ast->content.data.str)) > -1){
            return execute_defined_func(ast, defined, let_map, idx, max_depth - 1);
        }
        else if(ast->size == 1){
            struct Value a = execute(ast->children[0], defined, let_map,  max_depth-1);
            if(ast->type == 'k'){
                if(string_matches(&ast->content.data.str, &print_const)){
                    print(a);
                    printf("\n");
                    result.type = 'u';
                }
                else if(string_matches(&ast->content.data.str, &length_const)){
                    return length(a);
                }
                else if(string_matches(&ast->content.data.str, &return_const)){
                    return execute(ast->children[0], defined, let_map, max_depth-1);
                }
            }
        }
        else if(ast->size == 2) {
            struct Value a = execute(ast->children[0], defined, let_map, max_depth-1);
            struct Value b = execute(ast->children[1], defined, let_map, max_depth-1);
            result = apply_core_function(ast, a, b);
        } else {
            result = reduce_ast(ast, defined, let_map, max_depth - 1);
        }
    }

    return result;
}

struct Value execute_defined_func(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int idx, int max_depth){
    struct Map * arguments = make_args_map(ast, defined, idx);
    struct Tree * populated_ast = populate_args(arguments, duplicate_tree(get_defined_body(defined->trees[idx])));
    return execute(populated_ast, defined, let_map, max_depth-1);
}
