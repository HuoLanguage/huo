#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"

void store_let_value(struct Value * key, struct Value * value, struct Scopes * scopes){
    struct Map * let_store = scopes->scopes[scopes->current];
    struct Keyval * let_binding = malloc(sizeof(struct Keyval));
    if (key->type != KEYWORD) {
        ERROR("Invalid type for let keyword: '%c'", key->type);
    }
    let_binding->key = value_copy_heap(key);
    let_binding->val = value_copy_heap(value);
    int index = -1;
    for(int i = 0; i < let_store->size; i++){
        if(string_matches(&let_binding->key->data.str, &let_store->members[i]->key->data.str)){
            index = i;
        }
    }
    if(index > -1){
        let_store->members[index] = let_binding;
    } else {
        let_store->members[let_store->size] = let_binding;
        let_store->size++;
    }
}

void store_let_binding(struct Tree * key, struct Tree * value, struct Tree_map * defined, struct Scopes * scopes, int max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    struct Value val = execute(value, defined, scopes, max_depth - 1);
    store_let_value(&key->content, &val, scopes);
}
