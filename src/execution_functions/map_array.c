#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "let_binding.h"
#include "../config.h"

struct Value map_array(struct Execution_bundle * exec_bundle){
    struct Tree * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    bool use_index;
    size_t func_index;
    if (ast->size == 4) {
        use_index = true;
        func_index = 3;
    } else if (ast->size == 3) {
        use_index = false;
        func_index = 2;
    } else {
        ERROR("Wrong number of arguments for map__array: %zu != [3,4]\n", ast->size);
    }
    exec_bundle->ast = ast->children[0];
    struct Value array = execute(exec_bundle);
    if (array.type != ARRAY) {
        ERROR("Wrong type for map: '%c' != ARRAY", array.type);
    }
    for(size_t i = 0; i < array.data.array->size; i++){
        struct Value *item = value_copy_heap(array.data.array->values[i]);
        struct Tree * function = duplicate_tree(ast->children[func_index]);
        store_let_value(&ast->children[1]->content, item, exec_bundle->scopes);
        if (use_index) {
            struct Value index = value_from_long(i);
            store_let_value(&ast->children[2]->content, &index, exec_bundle->scopes);
        }
        exec_bundle->ast = function;
        struct Value result = execute(exec_bundle);
        array.data.array->values[i] = value_copy_heap(&result);
    }
    return array;
}
