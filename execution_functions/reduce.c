#include <stdio.h>
#include <stdlib.h>
#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "populate_reduce_function.h"
#include "../core_functions.h"

struct Value reduce_array(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    if (ast->size < 4) {
        perror("Not enough arguments for reduce_array\n");
        exit(0);
    }
    int start = 0;
    struct Value array = execute(ast->children[0], defined, let_map);
    struct Value result;
    if(ast->size == 5){
        result = execute(ast->children[4], defined, let_map);
    } else {
        result = * array.data.array->values[0];
        start = 1;
    }
    for(int i = start; i < array.data.array->size; i++){
        struct Value * item = array.data.array->values[i];
        struct Tree * function = duplicate_tree(ast->children[3]);

        populate_reduce_function(&ast->children[1]->content, &ast->children[2]->content, function, item, &result);
        result = execute(function, defined, let_map);
    }
    return result;
}
