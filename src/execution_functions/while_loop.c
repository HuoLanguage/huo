#include "../structures/structures.h"
#include "../constants.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

void while_loop(struct Tree * ast, hash_table *defined, struct Scopes * scopes, int max_depth){
    if(ast->size < 2){
        ERROR("Not enough arguments for while: %i < 2\n", ast->size);
    }
    for(int i = 0; i < LOOP_MAX; i++){
        struct Value condition = execute(ast->children[0], defined, scopes, max_depth - 1);
        if(value_as_bool(condition)){
            execute(ast->children[1], defined, scopes, max_depth - 1);
        } else {
            break;
        }
    }
}
