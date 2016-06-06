#include <stdio.h>
#include "structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "constants.h"
#include "execute.h"

struct Value reduce(struct Tree * ast, struct Tree_map * defined){
    struct Value result = execute(ast->children[0], defined);

    for(int i = 1; i < ast->size; i++){
        if(ast->children[i]->type == 'f'){
            struct Value newResult = execute(ast->children[i], defined);
            result = apply_core_function(ast, result, newResult);
        } else {
            result = apply_core_function(ast, result, ast->children[i]->content);
        }
    }
    return result;
}

struct Value execute (struct Tree * ast, struct Tree_map * defined){
    struct Value result;
    // first check for special kinds of execution
    if(string_matches(ast->content.data.str, if_const)){
        result = execute(ast->children[0], defined);
        if(result.type != 'b'){
            // throw error
        }
        else if(result.data.bl == 't'){ // result is boolean true
            result = execute(ast->children[1], defined);
        }
        else if(result.data.bl == 'f'){ // result is boolean false
            result = execute(ast->children[2], defined);
        }
        return result;
    }

    // then check for function invocations
    int idx;
    if(!ast->size){
        return ast->content;
    }
    else if(ast->type == 'k' && (idx = is_defined_func(defined, ast->content.data.str)) > -1){
        return execute_defined_func(ast, defined, idx);
    }
    else if(ast->size == 1){
        struct Value a = execute(ast->children[0], defined);
        if(ast->type == 'k'){
            if(string_matches(ast->content.data.str, print_const)){
                print(a);
                printf("\n");
            }
        }
    }
    else if(ast->size == 2) {
        struct Value a = execute(ast->children[0], defined);
        struct Value b = execute(ast->children[1], defined);
        result = apply_core_function(ast, a, b);
    } else {
        result = reduce(ast, defined);
    }
    return result;
}

struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b){
    struct Value result;

    if(ast->type == 'k'){
        if(string_matches(ast->content.data.str, concat_const)){
            result = concat(a, b);
        }
    }
    else if(ast->type == '*'){
        result = mul(a, b);
    }
    else if(ast->type == '+'){
        result = add(a, b);
    }
    else if(ast->type == '-'){
        result = sub(a, b);
    }
    else if(ast->type == '/'){
        result = divide(a, b);
    }
    else if(ast->type == '!'){
        result = not(a, b);
    }
    else if(ast->type == '='){
        result = equals(a, b);
    }
    return result;
}

struct Value execute_defined_func(struct Tree * ast, struct Tree_map * defined, int idx){
    struct Map * arguments = make_args_map(ast, defined, idx);
    struct Tree * populated_ast = populate_args(arguments, get_defined_body(defined->trees[idx]));
    return execute(populated_ast, defined);
}
