#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

struct Value reduce_ast(struct Execution_bundle * exec_bundle){
    huo_ast * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast_size(ast) < 1) {
        ERROR("Not enough arguments for reduce_ast: %zu < 2\n", ast_size(ast));
    }
    struct Value result;

    for(size_t i = 0; i < ast_size(ast); i++){
        exec_bundle->ast = ast_child(ast, i);
        result = execute(exec_bundle);
    }
    return result;
}
