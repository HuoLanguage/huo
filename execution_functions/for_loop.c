#include <stdio.h>
#include <stdlib.h>
#include "../structures.h"
#include "../execute.h"

void for_loop(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    if (ast->size < 3) {
        perror("Not enough arguments for for_loop\n");
        exit(1);
    }
    struct Value start = execute(ast->children[0], defined, let_map);
    struct Value end = execute(ast->children[1], defined, let_map);
    if(start.data.ln > end.data.ln){
        for(long i = start.data.ln; i > end.data.ln; i--){
            execute(ast->children[2], defined, let_map);
        }
    } else {
        for(long i = start.data.ln; i < end.data.ln; i++){
            execute(ast->children[2], defined, let_map);
        }
    }
}
