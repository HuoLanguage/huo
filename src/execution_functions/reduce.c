#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "let_binding.h"
#include "../core_functions.h"
#include "../config.h"

struct Value reduce_array(struct Execution_bundle * exec_bundle){
    huo_ast * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast_size(ast) != 5 && ast_size(ast) != 6) {
        ERROR("Wrong number of arguments for reduce_array: %zu != [5, 6]\n", ast_size(ast));
    }
    if (ast_type(ast_child(ast, 2)) != AST_KEYWORD) {
        ERROR("Invalid type for reduce_array: '%c' != 'k'\n", ast_type(ast_child(ast, 2)));
    }
    if (ast_type(ast_child(ast, 3)) != AST_KEYWORD) {
        ERROR("Invalid type for reduce_array: '%c' != 'k'\n", ast_type(ast_child(ast, 3)));
    }
    size_t start;
    struct Value result;
    exec_bundle->ast = ast_child(ast, 1);
    struct Value array = execute(exec_bundle);
    if (array.type != ARRAY) {
        ERROR("Invalid type for reduce_array: '%c' != ARRAY\n", array.type);
    }
    if (array.data.array->size <= 0) {
        return value_copy_stack(&array);
    }
    if(ast_size(ast) == 6){
        exec_bundle->ast = ast_child(ast, 5);
        result = execute(exec_bundle);
        start = 0;
    } else {
        result = *array.data.array->values[0];
        start = 1;
    }
    for(size_t i = start; i < array.data.array->size; i++){
        struct Value * item = array.data.array->values[i];
        huo_ast * function = ast_copy(ast_child(ast, 4));
        store_let_value(ast_value(ast_child(ast, 2)), &result, exec_bundle->scopes);
        store_let_value(ast_value(ast_child(ast, 3)), item, exec_bundle->scopes);
        exec_bundle->ast = function;
        result = execute(exec_bundle);
    }
    exec_bundle->ast = ast;
    return result;
}
