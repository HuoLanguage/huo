#include <stdio.h>
#include <stdlib.h>
#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "populate_each_function.h"

struct Value map_array(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    if (ast->size < 4) {
        perror("Not enough arguments for map_array\n");
        exit(1);
    }
    struct Value array = execute(ast->children[0], defined, let_map);
    for(int i = 0; i < array.data.array->size; i++){
        struct Value * item = array.data.array->values[i];
        struct Value index = {
            .type = 'l',
            .data = {
                .ln=(long)i
            }
        };
        struct Tree * function = duplicate_tree(ast->children[3]);
        populate_each_function(&ast->children[1]->content, &ast->children[2]->content, function, item, &index);
        struct Value result = execute(function, defined, let_map);
        copy_value(item, &result);
    }
    return array;
}
