#include "../structures/structures.h"
#include "../execute.h"
#include "populate_each_function.h"
#include "../base_util.h"

struct Value for_each(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 4) {
        ERROR("Not enough arguments for for_each: %i < 4\n", ast->size);
    }
    struct Value array = execute(ast->children[0], defined, let_map, max_depth - 1);
    if (array.type != 'a') {
        ERROR("Invalid type for for_each iterable: '%c' != 'a'", array.type);
    }
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
        execute(function, defined, let_map, max_depth - 1);
    }
    return array;
}
