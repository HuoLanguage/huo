#include <stdio.h>
#include <stdlib.h>
#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"

void store_let_binding(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    if (ast->size < 2) {
        perror("Not enough arguments for store_let_binding\n");
        exit(1);
    }
    struct Keyval * let_binding = malloc(sizeof(struct Keyval));
    let_binding->key = malloc(sizeof(struct Value));
    copy_value(let_binding->key, &ast->children[0]->content);
    struct Value val = execute(ast->children[1], defined, let_map);
    struct Value * val_ptr = malloc(sizeof(struct Value));
    copy_value(val_ptr, &val);
    let_binding->val = val_ptr;
    int index = -1;
    for(int i = 0; i < let_map->size; i++){
        if(string_matches(let_binding->key->data.str, let_map->members[i]->key->data.str)){
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
