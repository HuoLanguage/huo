#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "populate_each_function.h"

struct Value map_array(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    if (ast->size < 4) {
        ERROR("Not enough arguments for map_array: %i < 4\n", ast->size);
    }
    struct Value array = execute(ast->children[0], defined, let_map);
    if (array.type != 'a') {
        ERROR("Wrong type for map: '%c' != 'a'", array.type);
    }
    for(int i = 0; i < array.data.array->size; i++){
        struct Value *item = array.data.array->values[i];
        struct Value index = {
            .type = 'l',
            .data = {
                .ln=(long)i
            }
        };
        struct Tree * function = duplicate_tree(ast->children[3]);
        populate_each_function(&ast->children[1]->content, &ast->children[2]->content, function, item, &index);
        struct Value result = execute(function, defined, let_map);
        array.data.array->values[i] = copy_value_heap(&result);
    }
    return array;
}
