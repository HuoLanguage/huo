#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../core_functions.h"
#include "../config.h"

void store_let_value(struct Value * key, struct Value * value, struct Scopes * scopes){

    hash_table *let_store = scopes->scopes[scopes->current];
    struct definition *def = malloc_or_die(sizeof(struct definition));
    def->is_func = false;
    def->val.val = value_copy_heap(value);
    struct String val = value_as_keyword(key);
    //printf("val=%s --> ", string_to_chars(&val));
    //print(*value);
    //printf("\n");
    if (hash_table_put(let_store, string_copy_heap(&val), def)) {
        // Overriding
    } else {
        // New addition
    }
}
void store_def_func(struct Value * key, huo_ast *func, struct Scopes * scopes){
    hash_table *let_store = scopes->scopes[scopes->current];
    struct definition *def = malloc_or_die(sizeof(struct definition));
    def->is_func = true;
    def->val.ast = func;
    struct String val = value_as_keyword(key);
    if (hash_table_put(let_store, string_copy_heap(&val), def)) {
        // Overriding
    } else {
        // New addition
    }
}
