#include "../structures/structures.h"
#include "../constants.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"
#include "../config.h"

struct Value switch_case(struct Execution_bundle * exec_bundle){
    huo_ast * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    for(size_t i = 2; i < ast_size(ast); i++){
        huo_ast * routine = ast_child(ast, i);
        if(ast_value(ast_child(routine, 0))->type == TYPE_KEYWORD && string_matches_heap(&default_const, &ast_value(ast_child(routine, 0))->data.str)) {
            if (i != ast_size(ast) - 1) {
                ERROR("Default not last case!");
            }
            exec_bundle->ast = ast_child(routine, 1);
            struct Value ret = execute(exec_bundle);
            exec_bundle->ast = ast;
            return ret;
        }
        if (ast_size(routine) != 3) {
            ERROR("Invalid syntax for switch_case: %zu != 3", ast_size(routine));
        }
        huo_ast * return_value = ast_child(routine, 2);
        ast_set_child(routine, 2, ast_child(ast, 1));
        exec_bundle->ast = routine;
        struct Value result = execute(exec_bundle);
        if(value_as_bool(&result)){
            exec_bundle->ast = return_value;
            struct Value ret = execute(exec_bundle);
            exec_bundle->ast = ast;
            return ret;
        }
    }
    exec_bundle->ast = ast;
    struct Value result;
    result.type = TYPE_UNDEF;
    return result;
}
