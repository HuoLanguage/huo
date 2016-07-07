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
#include "execution_functions/if_block.h"
#include "execution_functions/let_binding.h"
#include "execution_functions/switch.h"
#include "execution_functions/parallel_execution.h"
#include "execution_functions/while_loop.h"


struct Value apply_execution_function(struct Tree * ast, hash_table * defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth){
    struct Value undefined = {
        .type = UNDEF
    };
    if(ast->content.type == KEYWORD && string_matches_heap(&ast->content.data.str, &if_const)){
        return if_block(ast, defined, scopes, function_names, max_depth-1);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&let_const, &ast->content.data.str)){
        if (ast->size < 2) {
            ERROR("Not enough arguments for store_let_binding: %i < 2\n", ast->size);
        }
        store_let_binding(ast->children[0],ast->children[1], defined, scopes, function_names, max_depth-1);
        return undefined;
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&each_const, &ast->content.data.str)){
        for_each(ast, defined, scopes, function_names, max_depth-1);
        return undefined;
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&map_const, &ast->content.data.str)){
        return map_array(ast, defined, scopes, function_names, max_depth-1);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&while_const, &ast->content.data.str)){
        while_loop(ast, defined, scopes, function_names, max_depth - 1);
        return undefined;
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&reduce_const, &ast->content.data.str)){
        return reduce_array(ast, defined, scopes, function_names, max_depth - 1);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&set_const, &ast->content.data.str)) {
        if (ast->size < 3) {
            ERROR("Not enough arguments for set: %i < 3", ast->size);
        }
        struct Value index = execute(ast->children[0], defined, scopes, function_names, max_depth-1);
        struct Value item = execute(ast->children[1], defined, scopes, function_names, max_depth-1);
        struct Value array = execute(ast->children[2], defined, scopes, function_names, max_depth-1);

        return set(index, item, array);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&for_const, &ast->content.data.str)){
        for_loop(ast, defined, scopes, function_names, max_depth - 1);
        return undefined;
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&do_const, &ast->content.data.str)){
        if (!ast->size) {
            ERROR("Not enough arguments for do: %i < 1", ast->size);
        }
        for(int i = 0; i < ast->size; i++){
            if(i == ast->size-1){
                return execute(ast->children[i], defined, scopes, function_names, max_depth-1);
            } else {
                execute(ast->children[i], defined, scopes, function_names, max_depth-1);
            }
        }
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&read_const, &ast->content.data.str)){
        if (ast->size < 1) {
            ERROR("Not enough arguments for read: %i < 1", ast->size);
        }
        char t = ast->children[0]->content.type;
        if (t != STRING && t != KEYWORD) {
            ERROR("Invalid type for read: '%c' != [STRING, KEYWORD]", t);
        }
        return value_from_string(read_file(ast->children[0]->content.data.str));
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&substring_const, &ast->content.data.str)){
        if (ast->size < 3) {
            ERROR("Not enough arguments for substring: %i < 3", ast->size);
        }
        struct Value string = execute(ast->children[2], defined, scopes, function_names, max_depth-1);
        struct Value start = execute(ast->children[0], defined, scopes, function_names, max_depth-1);
        struct Value end = execute(ast->children[1], defined, scopes, function_names, max_depth-1);
        
        return substring(start, end, string);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&switch_const, &ast->content.data.str)){
        return switch_case(ast, defined, scopes, function_names, max_depth-1);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&parallel_const, &ast->content.data.str)){
        parallel_execution(ast, defined, scopes, function_names, max_depth-1);
        return undefined;
    }
    return undefined;
}
