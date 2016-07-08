#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

struct Value if_block(struct Execution_bundle * exec_bundle){
    struct Tree * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size != 2 && ast->size != 3) {
        ERROR("Wrong number of arguments for if_block: %zu != [2,3]\n", ast->size);
    }
    exec_bundle->ast = ast->children[0];
    struct Value result = execute(exec_bundle);
    if(value_as_bool(&result)){ // result is boolean true
        exec_bundle->ast = ast->children[1];
        result = execute(exec_bundle);
    }
    else if (ast->size == 3) { // result is boolean false
        exec_bundle->ast = ast->children[2];
        result = execute(exec_bundle);
    } else {
        result = value_from_undef();
    }
    return result;
}
