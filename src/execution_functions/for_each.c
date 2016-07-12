#include <stdbool.h>
#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"
#include "let_binding.h"
#include "for_each.h"
#include "../config.h"

struct Value for_each(struct Execution_bundle * exec_bundle) {
    huo_ast * ast = exec_bundle->ast;
    struct Scopes * scopes = exec_bundle->scopes;

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
        ERROR("Wrong number of arguments for for_each: %zu != [5,4]\n", ast_size(ast));
    }
    exec_bundle->ast = ast_child(ast, 1);
    struct Value array = execute(exec_bundle);
    if(array.type == STRING){
        exec_bundle->ast = ast;
        return for_each_string(value_as_string(&array), exec_bundle);
    } else if (array.type == ARRAY) {
        for(size_t i = 0; i < array.data.array->size; i++){
            struct Value *item = array.data.array->values[i];
            huo_ast * function = ast_copy(ast_child(ast, func_index));
            store_let_value(ast_value(ast_child(ast, 2)), item, scopes);
            if (use_index) {
                struct Value index = value_from_long(i);
                store_let_value(ast_value(ast_child(ast, 3)), &index, scopes);
            }
            exec_bundle->ast = function;
            execute(exec_bundle);
        }
        return array;
    } else {
        ERROR("Invalid type for for_each iterable: '%c' != ARRAY", array.type);
    }
}

struct Value for_each_string(struct String string, struct Execution_bundle * exec_bundle){
    huo_ast * ast = exec_bundle->ast;
    struct Scopes * scopes = exec_bundle->scopes;

    bool use_index;
    size_t func_index;
    if (ast_size(ast) == 5) {
        use_index = true;
        func_index = 4;
    } else if (ast_size(ast) == 4) {
        use_index = false;
        func_index = 3;
    } else {
        ERROR("Wrong number of arguments for for_each: %zu != [5,4]\n", ast_size(ast));
    }

    for(size_t i = 0; i < string.length; i++){
        struct String item = string_substring(i, i+1, string);
        struct Value item_val = value_from_string(string_copy_stack(&item));
        huo_ast * function = ast_copy(ast_child(ast, func_index));
        store_let_value(ast_value(ast_child(ast, 2)), &item_val, scopes);
        if (use_index) {
            struct Value index = value_from_long(i);
            store_let_value(ast_value(ast_child(ast, 3)), &index, scopes);
        }
        exec_bundle->ast = function;
        execute(exec_bundle);
    }
    return value_from_string(string);
}
