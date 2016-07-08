#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

struct Value reduce_ast(struct Execution_bundle * exec_bundle){
    struct Tree * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 1) {
        ERROR("Not enough arguments for reduce_ast: %zu < 2\n", ast->size);
    }
    exec_bundle->ast = ast->children[0];
    struct Value result = execute(exec_bundle);

    for(size_t i = 1; i < ast->size; i++){
        if(ast->children[i]->type == 'f'){
            exec_bundle->ast = ast->children[i];
            struct Value newResult = execute(exec_bundle);
            result = apply_core_function(ast, result, newResult);
        } else {
            result = apply_core_function(ast, result, ast->children[i]->content);
        }
    }
    return result;
}
