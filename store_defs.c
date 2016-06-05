#include "structures.h"

int store_defs(struct Tree * ast, struct Tree_map * defined){
    int num_defs = 0;
    for(int i = 0; i < ast->size; i++){
        if(string_matches(ast->children[i]->content.data.str, def_const)){
            defined->names[defined->size] = &ast->children[i]->children[0]->content.data.str;
            defined->trees[defined->size] = ast->children[i];
            defined->size++;
            num_defs++;
        }
    }
    return num_defs;
}

int is_defined_func(struct Tree_map * defined, struct String key){
    for(int i = 0; i < defined->size; i++){
        if(string_matches(*defined->names[i], key)){
            return i;
        }
    }
    return -1;
}
