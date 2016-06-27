#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"

void for_loop(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes){
    if (ast->size < 3) {
        ERROR("Not enough arguments for for_loop: %i < 3\n", ast->size);
    }
    struct Value start = execute(ast->children[0], defined, scopes);
    struct Value end = execute(ast->children[1], defined, scopes);
    if(start.data.ln > end.data.ln){
        for(long i = start.data.ln; i > end.data.ln; i--){
            execute(ast->children[2], defined, scopes);
        }
    } else {
        for(long i = start.data.ln; i < end.data.ln; i++){
            execute(ast->children[2], defined, scopes);
        }
    }
}
