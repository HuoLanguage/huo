#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"

struct Value if_block(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes){
    if (ast->size < 3) {
        ERROR("Not enough arguments for if_block: %i < 3\n", ast->size);
    }
    struct Value result = execute(ast->children[0], defined, scopes);
    if(result.type != 'b'){
        ERROR("Invalid result type for if statement conditional: %c", result.type);
    }
    else if(result.data.bl == 't'){ // result is boolean true
        result = execute(ast->children[1], defined, scopes);
    }
    else if(result.data.bl == 'f'){ // result is boolean false
        result = execute(ast->children[2], defined, scopes);
    }
    return result;
}
