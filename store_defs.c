#include "structures.h"
#include "constants.h"

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

void copy_defs(struct Tree_map * imported, struct Tree_map * defined){
    for(int i = 0; i < imported->size; i++){
        defined->names[defined->size] = imported->names[i];
        defined->trees[defined->size] = imported->trees[i];
        defined->size++;
    }
}
