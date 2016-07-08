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


struct Value apply_execution_function(struct Execution_bundle * exec_bundle){
    struct Tree * ast = exec_bundle->ast;

    struct Value undefined = {
        .type = UNDEF
    };
    if(ast->content.type == KEYWORD && string_matches_heap(&ast->content.data.str, &if_const)){
        return if_block(exec_bundle);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&let_const, &ast->content.data.str)){
        if (ast->size < 2) {
            ERROR("Not enough arguments for store_let_binding: %zu < 2\n", ast->size);
        }
        store_let_binding(ast->children[0], ast->children[1], exec_bundle);
        return undefined;
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&each_const, &ast->content.data.str)){
        for_each(exec_bundle);
        return undefined;
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&map_const, &ast->content.data.str)){
        return map_array(exec_bundle);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&while_const, &ast->content.data.str)){
        while_loop(exec_bundle);
        return undefined;
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&reduce_const, &ast->content.data.str)){
        return reduce_array(exec_bundle);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&set_const, &ast->content.data.str)) {
        if (ast->size < 3) {
            ERROR("Not enough arguments for set: %zu < 3", ast->size);
        }
        exec_bundle->ast = ast->children[0];
        struct Value index = execute(exec_bundle);

        exec_bundle->ast = ast->children[1];
        struct Value item = execute(exec_bundle);

        exec_bundle->ast = ast->children[2];
        struct Value array = execute(exec_bundle);

        return set(index, item, array);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&for_const, &ast->content.data.str)){
        for_loop(exec_bundle);
        return undefined;
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&do_const, &ast->content.data.str)){
        if (!ast->size) {
            ERROR("Not enough arguments for do: %zu < 1", ast->size);
        }
        for(size_t i = 0; i < ast->size; i++){
            exec_bundle->ast = ast->children[i];
            if(i == ast->size-1){
                return execute(exec_bundle);
            } else {
                execute(exec_bundle);
            }
        }
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&read_const, &ast->content.data.str)){
        if (ast->size < 1) {
            ERROR("Not enough arguments for read: %zu < 1", ast->size);
        }
        char t = ast->children[0]->content.type;
        if (t != STRING && t != KEYWORD) {
            ERROR("Invalid type for read: '%c' != [STRING, KEYWORD]", t);
        }
        return value_from_string(read_file(ast->children[0]->content.data.str));
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&substring_const, &ast->content.data.str)){
        if (ast->size < 3) {
            ERROR("Not enough arguments for substring: %zu < 3", ast->size);
        }
        exec_bundle->ast = ast->children[0];
        struct Value start = execute(exec_bundle);
        exec_bundle->ast = ast->children[1];
        struct Value end = execute(exec_bundle);
        exec_bundle->ast = ast->children[2];
        struct Value string = execute(exec_bundle);

        return substring(start, end, string);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&switch_const, &ast->content.data.str)){
        return switch_case(exec_bundle);
    }
    else if(ast->content.type == KEYWORD && string_matches_heap(&parallel_const, &ast->content.data.str)){
        parallel_execution(exec_bundle);
        return undefined;
    }
    return undefined;
}
