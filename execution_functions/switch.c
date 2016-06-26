#include "../structures.h"
#include "../constants.h"
#include "../execute.h"
#include "../base_util.h"
#include "populate_reduce_function.h"
#include "../core_functions.h"

struct Value switch_case(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    for(int i = 1; i < ast->size; i++){
        struct Tree * routine = ast->children[i];
        if(routine->type == 'k' && string_matches(&default_const, &routine->content.data.str)){
            return execute(routine->children[0], defined, let_map);
        }
        struct Tree * return_value = routine->children[1];
        routine->children[1] = ast->children[0];
        struct Value result = execute(routine, defined, let_map);
        if(result.type != 'b'){
            ERROR("Switch test return non-boolean value (%c != 'b')", result.type);
        } else {
            if(result.data.bl == 't'){
                return execute(return_value, defined, let_map);
            }
        }
    }
    struct Value result;
    result.type = 'u';
    return result;
}
