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
#include "apply_core_function.h"

struct Value execute (struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    struct Value result;
    // first check for special kinds of execution
    if(string_matches(ast->content.data.str, if_const)){
        return if_block(ast, defined, let_map);
    }
    if(string_matches(let_const, ast->content.data.str)){
        store_let_binding(ast, defined, let_map);
        result.type = 'u';
        return result;
    }
    if(string_matches(each_const, ast->content.data.str)){
        for_each(ast, defined, let_map);
        result.type = 'u';
        return result;
    }
    if(string_matches(map_const, ast->content.data.str)){
        return map_array(ast, defined, let_map);
    }
    if(string_matches(reduce_const, ast->content.data.str)){
        return reduce_array(ast, defined, let_map);
    }
    if(string_matches(set_const, ast->content.data.str)){
        struct Value index = execute(ast->children[0], defined, let_map);
        struct Value item = execute(ast->children[1], defined, let_map);
        struct Value array = execute(ast->children[2], defined, let_map);
        result = array_set(index, item, array);
        return result;
    }
    if(string_matches(for_const, ast->content.data.str)){
        for_loop(ast, defined, let_map);
        result.type = 'u'; //return undefined
        return result;
    }
    if(string_matches(do_const, ast->content.data.str)){
        for(int i = 0; i < ast->size; i++){
            if(i == ast->size-1){
                result = execute(ast->children[i], defined, let_map);
            } else {
                execute(ast->children[i], defined, let_map);
            }
        }
        return result;
    }
    if(string_matches(read_const, ast->content.data.str)){
        return read_file(ast);
    }

    int idx;
    if(!ast->size){
        if(ast->type == 'k'){
            for(int i = 0; i < let_map->size; i++){
              if(string_matches(let_map->members[i]->key->data.str, ast->content.data.str)){
                  return *let_map->members[i]->val;
              }
            }
        } else {
            return ast->content;
        }
    }
    else if(ast->type == 'k' && (idx = is_defined_func(defined, ast->content.data.str)) > -1){
        return execute_defined_func(ast, defined, let_map, idx);
    }
    else if(ast->size == 1){
        struct Value a = execute(ast->children[0], defined, let_map);
        if(ast->type == 'k'){
            if(string_matches(ast->content.data.str, print_const)){
                print(a);
                printf("\n");
            }
            if(string_matches(ast->content.data.str, length_const)){
                return length(a);
            }
        }
    }
    else if(ast->size == 2) {
        struct Value a = execute(ast->children[0], defined, let_map);
        struct Value b = execute(ast->children[1], defined, let_map);
        result = apply_core_function(ast, a, b);
    } else {
        result = reduce_ast(ast, defined, let_map);
    }
    return result;
}

struct Value execute_defined_func(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int idx){
    struct Map * arguments = make_args_map(ast, defined, idx);
    struct Tree * populated_ast = populate_args(arguments, get_defined_body(defined->trees[idx]));
    return execute(populated_ast, defined, let_map);
}
