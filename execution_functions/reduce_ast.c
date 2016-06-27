#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"

struct Value reduce_ast(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 1) {
        ERROR("Not enough arguments for reduce_ast: %i < 2\n", ast->size);
    }
    struct Value result = execute(ast->children[0], defined, scopes, max_depth - 1);

    for(int i = 1; i < ast->size; i++){
        if(ast->children[i]->type == 'f'){
            struct Value newResult = execute(ast->children[i], defined, scopes, max_depth - 1);
            result = apply_core_function(ast, result, newResult);
        } else {
            result = apply_core_function(ast, result, ast->children[i]->content);
        }
    }
    return result;
}
