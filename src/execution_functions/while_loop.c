#include <stdbool.h>
#include "../structures/structures.h"
#include "../constants.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

void while_loop(struct Execution_bundle * exec_bundle){
    huo_ast * ast = exec_bundle->ast;
    if(ast_size(ast) != 3){
        ERROR("Not enough arguments for while: %zu < 3\n", ast_size(ast));
    }
    huo_int_t i = 0;
    while (true) {
        if (LOOP_MAX != (huo_int_t) -1) {
            if (i++ >= LOOP_MAX) {
                break;
            }
        }
        exec_bundle->ast = ast_child(ast, 1);
        struct Value condition = execute(exec_bundle);
        if(value_as_bool(&condition)){
            exec_bundle->ast = ast_child(ast, 2);
            execute(exec_bundle);
        } else {
            return;
        }
    }
    ERROR("Max loops exceeded");
}
