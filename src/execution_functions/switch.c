#include "../structures/structures.h"
#include "../constants.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"
#include "../config.h"

struct Value switch_case(struct Execution_bundle * exec_bundle){
    struct Tree * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    for(size_t i = 1; i < ast->size; i++){
        struct Tree * routine = ast->children[i];
        if(routine->type == 'k' && string_matches_heap(&default_const, &routine->content.data.str)){
            exec_bundle->ast = routine->children[0];
            return execute(exec_bundle);
        }
        if (routine->size != 2) {
            ERROR("Invalid syntax for switch_case: %zu != 2", routine->size);
        }
        
        struct Tree * return_value = routine->children[1];
        routine->children[1] = ast->children[0];
        exec_bundle->ast = routine;
        struct Value result = execute(exec_bundle);
        if(value_as_bool(&result)){
            exec_bundle->ast = return_value;
            return execute(exec_bundle);
        }
    }
    struct Value result;
    result.type = UNDEF;
    return result;
}
