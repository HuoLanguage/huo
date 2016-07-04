#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

struct Value if_block(struct Tree * ast, hash_table *defined, struct Scopes * scopes, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 3) {
        ERROR("Not enough arguments for if_block: %i < 3\n", ast->size);
    }
    struct Value result = execute(ast->children[0], defined, scopes, max_depth - 1);
    if(value_as_bool(&result)){ // result is boolean true
        result = execute(ast->children[1], defined, scopes, max_depth - 1);
    }
    else { // result is boolean false
        result = execute(ast->children[2], defined, scopes, max_depth - 1);
    }
    return result;
}
