#include "../structures/structures.h"
#include "../constants.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"
#include "../config.h"

struct Value switch_case(struct Tree * ast, hash_table *defined, struct Scopes * scopes, huo_depth_t max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    for(size_t i = 1; i < ast->size; i++){
        struct Tree * routine = ast->children[i];
        if(routine->type == 'k' && string_matches_heap(&default_const, &routine->content.data.str)){
            return execute(routine->children[0], defined, scopes, max_depth - 1);
        }
        if (routine->size != 2) {
            ERROR("Invalid syntax for switch_case: %zu != 2", routine->size);
        }
        routine->children[1] = ast->children[0];
        struct Value result = execute(routine, defined, scopes, max_depth - 1);
        if(value_as_bool(&result)){
            struct Tree * return_value = routine->children[1];
            return execute(return_value, defined, scopes, max_depth - 1);
        }
    }
    struct Value result;
    result.type = UNDEF;
    return result;
}
