#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"

struct Value if_block(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 3) {
        ERROR("Not enough arguments for if_block: %i < 3\n", ast->size);
    }
    struct Value result = execute(ast->children[0], defined, let_map, max_depth - 1);
    if(result.type != BOOL){
        ERROR("Invalid result type for if statement conditional: %c", result.type);
    }
    else if(result.data.bl){ // result is boolean true
        result = execute(ast->children[1], defined, let_map, max_depth - 1);
    }
    else { // result is boolean false
        result = execute(ast->children[2], defined, let_map, max_depth - 1);
    }
    return result;
}
