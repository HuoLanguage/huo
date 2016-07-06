#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

struct Value if_block(struct Tree * ast, hash_table *defined, struct Scopes * scopes, struct Value_array * function_names, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size != 2 && ast->size != 3) {
        ERROR("Wrong number of arguments for if_block: %i != [2,3]\n", ast->size);
    }
    struct Value result = execute(ast->children[0], defined, scopes, function_names, max_depth - 1);
    if(value_as_bool(&result)){ // result is boolean true
        result = execute(ast->children[1], defined, scopes, function_names, max_depth - 1);
    }
    else if (ast->size == 3) { // result is boolean false
        result = execute(ast->children[2], defined, scopes, function_names, max_depth - 1);
    } else {
        result = value_from_undef();
    }
    return result;
}
