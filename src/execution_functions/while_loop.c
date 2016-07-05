#include <stdbool.h>
#include "../structures/structures.h"
#include "../constants.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

void while_loop(struct Tree * ast, hash_table *defined, struct Scopes * scopes, huo_depth_t max_depth){
    if(ast->size != 2){
        ERROR("Not enough arguments for while: %zu < 2\n", ast->size);
    }
    huo_int_t i = 0;
    while (true) {
        if (LOOP_MAX != (huo_int_t) -1) {
            if (i++ >= LOOP_MAX) {
                break;
            }
        }
        struct Value condition = execute(ast->children[0], defined, scopes, max_depth - 1);
        if(value_as_bool(&condition)){
            execute(ast->children[1], defined, scopes, max_depth - 1);
        } else {
            return;
        }
    }
    ERROR("Max loops exceeded");
}
