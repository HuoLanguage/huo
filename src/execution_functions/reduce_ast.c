#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

struct Value reduce_ast(struct Tree * ast, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 1) {
        ERROR("Not enough arguments for reduce_ast: %i < 2\n", ast->size);
    }
    struct Value result = execute(ast->children[0], defined, scopes, function_names, max_depth - 1);

    for(int i = 1; i < ast->size; i++){
        if(ast->children[i]->type == 'f'){
            struct Value newResult = execute(ast->children[i], defined, scopes, function_names, max_depth - 1);
            result = apply_core_function(ast, result, newResult);
        } else {
            result = apply_core_function(ast, result, ast->children[i]->content);
        }
    }
    return result;
}
