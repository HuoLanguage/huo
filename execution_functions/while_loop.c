#include "../structures.h"
#include "../constants.h"
#include "../execute.h"
#include "../base_util.h"

void while_loop(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes, int max_depth){
    if(ast->size < 2){
        ERROR("Not enough arguments for while: %i < 2\n", ast->size);
    }
    for(int i = 0; i < LOOP_MAX; i++){
        struct Value condition = execute(ast->children[0], defined, scopes, max_depth - 1);
        if(condition.type != 'b'){
            ERROR("First argument to 'while' function must return boolean: ('%c' != b)\n", condition.type);
        }
        if(condition.data.bl == 't'){
            execute(ast->children[1], defined, scopes, max_depth - 1);
        } else {
            break;
        }
    }
}
