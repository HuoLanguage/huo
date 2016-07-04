#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

void for_loop(struct Tree * ast, hash_table *defined, struct Scopes * scopes, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 3) {
        ERROR("Not enough arguments for for_loop: %i < 3\n", ast->size);
    }
    struct Value start = execute(ast->children[0], defined, scopes, max_depth - 1);
    struct Value end = execute(ast->children[1], defined, scopes, max_depth - 1);
    long start_i = value_as_long(&start);
    long end_i = value_as_long(&end);
    if(start_i > end_i){
        for(long i = start_i; i > end_i; i--){
            execute(ast->children[2], defined, scopes, max_depth - 1);
        }
    } else {
        for(long i = start_i; i < end_i; i++){
            execute(ast->children[2], defined, scopes, max_depth - 1);
        }
    }
}
