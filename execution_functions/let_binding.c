#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"

void store_let_binding(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size < 2) {
        ERROR("Not enough arguments for store_let_binding: %i < 2\n", ast->size);
    }
    struct Keyval * let_binding = malloc(sizeof(struct Keyval));
    if (ast->children[0]->type != 'k') {
        ERROR("Invalid type for let keyword: '%c'", ast->children[0]->type);
    }
    let_binding->key = copy_value_heap(&ast->children[0]->content);
    struct Value val = execute(ast->children[1], defined, let_map, max_depth - 1);
    let_binding->val = copy_value_heap(&val);
    int index = -1;
    for(int i = 0; i < let_map->size; i++){
        if(string_matches(&let_binding->key->data.str, &let_map->members[i]->key->data.str)){
            index = i;
        }
    }
    if(index > -1){
        let_map->members[index] = let_binding;
    } else {
        let_map->members[let_map->size] = let_binding;
        let_map->size++;
    }
}
