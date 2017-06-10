#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "let_binding.h"
#include "../config.h"

struct Value map_array(struct Execution_bundle * exec_bundle){
    huo_ast * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    bool use_index;
    size_t func_index;
    if (ast_size(ast) == 5) {
        use_index = true;
        func_index = 4;
    } else if (ast_size(ast) == 4) {
        use_index = false;
        func_index = 3;
    } else {
        ERROR("Wrong number of arguments for map_array: %zu != [4,5]\n", ast_size(ast));
    }
    exec_bundle->ast = ast_child(ast, 1);
    struct Value array = execute(exec_bundle);
    if (array.type != TYPE_ARRAY) {
        ERROR("Wrong type for map: '%c' != TYPE_ARRAY", array.type);
    }
    for(size_t i = 0; i < array.data.array->size; i++){
        struct Value *item = value_copy_heap(array.data.array->values[i]);
        huo_ast * function = ast_copy(ast_child(ast, func_index));
        store_let_value(ast_value(ast_child(ast, 2)), item, exec_bundle->scopes);
        if (use_index) {
            struct Value index = value_from_long(i);
            store_let_value(ast_value(ast_child(ast, 3)), &index, exec_bundle->scopes);
        }
        exec_bundle->ast = function;
        struct Value result = execute(exec_bundle);
        array.data.array->values[i] = value_copy_heap(&result);
    }
    exec_bundle->ast = ast;
    return array;
}
