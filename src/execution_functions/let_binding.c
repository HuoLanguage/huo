#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"
#include "../config.h"

void store_let_value(struct Value * key, struct Value * value, struct Scopes * scopes){
    hash_table *let_store = scopes->scopes[scopes->current];
    if (key->type != KEYWORD) {
        ERROR("Invalid type for let keyword: '%c'", key->type);
    }
    if (hash_table_put(let_store, value_copy_heap(key), value_copy_heap(value))) {
        // Overriding
    } else {
        // New addition
    }
}

void store_let_binding(struct Tree * key, struct Tree * value, struct Execution_bundle * exec_bundle){
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    exec_bundle->ast = value;
    struct Value val = execute(exec_bundle);
    store_let_value(&key->content, &val, exec_bundle->scopes);
}
