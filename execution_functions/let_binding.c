#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"

void store_let_value(struct Value * key, struct Value * value, struct Scopes * scopes){
    struct Map * let_store = scopes->scopes[scopes->current];
    struct Keyval * let_binding = malloc(sizeof(struct Keyval));
    let_binding->key = copy_value_heap(key);
    let_binding->val = copy_value_heap(value);
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

void store_let_binding(struct Tree * key, struct Tree * value, struct Tree_map * defined, struct Scopes * scopes){
    struct Value val = execute(value, defined, scopes);
    store_let_value(&key->content, &val, scopes);
}
