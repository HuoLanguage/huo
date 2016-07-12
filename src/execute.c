#include <stdio.h>
#include <stdlib.h>
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "constants.h"
#include "execute.h"
#include "execution_functions/reduce_ast.h"
#include "apply_core_function.h"
#include "apply_execution_function.h"
#include "apply_single_value_func.h"
#include "config.h"

struct Value execute(struct Execution_bundle * exec_bundle){
    exec_bundle->max_depth -= 1;

    huo_ast * ast = exec_bundle->ast;
    //printf("EXE: %s\n", string_to_chars(ast_to_string(ast)));
    struct Scopes * scopes = exec_bundle->scopes;
    huo_depth_t max_depth = exec_bundle->max_depth;

    struct Value result;
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if(ast_type(ast) == AST_ARRAY) {
        struct Value_array *arr = malloc_or_die(sizeof(struct Value_array));
        arr->size = ast_size(ast);
        arr->values = ARR_MALLOC(arr->size, struct Value *);
        for (size_t i = 0; i < arr->size; i++) {
            exec_bundle->ast = ast_child(ast, i);
            struct Value v = execute(exec_bundle);
            arr->values[i] = value_copy_heap(&v);
        }
        result = value_from_array(arr);
    }else if(!ast_size(ast)) {
        result = sub_vars(ast_value(ast), scopes, max_depth - 1);
    } else {
        huo_ast *func;
        struct Value *kwd = ast_value(ast_child(ast, 0));
        bool is_unbound_kwd = (kwd->type == KEYWORD);
        if(is_unbound_kwd && apply_execution_function(kwd, &result, exec_bundle)){
        // pass
        }
        else if(is_unbound_kwd && (func = get_defined_func(scopes, value_as_keyword(kwd))) != NULL) {
            make_args_map(exec_bundle, func);
            exec_bundle->ast = ast_copy(get_defined_body(func));
            result = execute(exec_bundle);
            scopes->current--;
        }
        else if(ast_size(ast) == 1){
            exec_bundle->ast = ast_child(ast, 0);
            result = execute(exec_bundle);
        }
        else if(is_unbound_kwd && ast_size(ast) == 2) {
            exec_bundle->ast = ast_child(ast, 1);
            //printf("'%s' = '", string_to_chars(ast_to_string(exec_bundle->ast)));
            struct Value a = execute(exec_bundle);
            //print(a);
            //printf("'\n");
            result = apply_single_value_func(kwd, exec_bundle, &a);
        }
        else if (is_unbound_kwd && ast_size(ast) == 3) {
            exec_bundle->ast = ast_child(ast, 1);
            struct Value a = execute(exec_bundle);

            exec_bundle->ast = ast_child(ast, 2);
            struct Value b = execute(exec_bundle);

            result = apply_core_function(kwd, exec_bundle, &a, &b);
        } else {
            result = reduce_ast(exec_bundle);
        }
    }

    exec_bundle->max_depth += 1;
    return result;
}
