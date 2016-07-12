#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

struct Value if_block(struct Execution_bundle * exec_bundle){
    huo_ast * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast_size(ast) != 3 && ast_size(ast) != 4) {
        ERROR("Wrong number of arguments for if_block: %zu != [3,4]\n", ast_size(ast));
    }
    exec_bundle->ast = ast_child(ast, 1);
    struct Value result = execute(exec_bundle);
    if(value_as_bool(&result)){ // result is boolean true
        exec_bundle->ast = ast_child(ast, 2);
        result = execute(exec_bundle);
    }
    else if (ast_size(ast) == 4) { // result is boolean false
        exec_bundle->ast = ast_child(ast, 3);
        result = execute(exec_bundle);
    } else {
        result = value_from_undef();
    }
    return result;
}
