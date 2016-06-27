#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "base_util.h"
#include "core_functions.h"
#include "process_defs.h"
#include "execute.h"
#include "execution_functions/let_binding.h"

void make_args_map(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes, int idx, int max_depth){
    // we want to evaluate the values passed into the function
    // but store the result in the next scope, not the current one
    struct Value vals[ast->size];
    for(int i = 0; i < ast->size; i++){
        vals[i] = execute(ast->children[i], defined, scopes, max_depth - 1);
    }
    make_scope(scopes);
    for(int l = 0; l < ast->size; l++){
        store_let_value(&defined->trees[idx]->children[l+1]->content, &vals[l], scopes);
    }
}

struct Tree * get_defined_body(struct Tree * function){
    // just pulls the function body out of a (def...)
    int check = 1;
    int index = 1;
    while(check){
        if (function->size <= index) {
            ERROR("No function body!");
        }
        if(!function->children[index]->size){
            index++;
        } else {
            check = 0;
            function = function->children[index];
        }
    }
    return function;
}

int is_defined_func(struct Tree_map * defined, struct String key){
    assert(string_is_sane(&key));
    for(int i = 0; i < defined->size; i++){
        if(string_matches(defined->names[i], &key)){
            return i;
        }
    }
    return -1;
}
