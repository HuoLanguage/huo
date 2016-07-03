#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "structures/structures.h"
#include "base_util.h"
#include "core_functions.h"

char *strdup(const char *str) {
    size_t len = strlen(str);
    char *dup = malloc(sizeof(char) * (len + 1));
    strcpy(dup, str);
    return dup;
}

void copy_array(struct Value * a, struct Value_array * b){
    struct Value_array * array = malloc(sizeof(struct Value_array));
    array->size = b->size;
    for(int i = 0; i < b->size; i++){
        array->values[i] = value_copy_heap(b->values[i]);
    }
    a->data.array = array;
}

struct Tree * duplicate_tree(struct Tree * a){
    struct Tree * root = malloc(sizeof(struct Tree));
    root->type = a->type;
    root->content = value_copy_stack(&a->content);
    root->children = NULL;
    root->size = a->size;
    RESIZE(root->children, root->size);
    for(int i = 0; i < a->size; i++){
        root->children[i] = duplicate_tree(a->children[i]);
    }
    return root;
}

void make_scope(struct Scopes * scopes){
    scopes->scopes[scopes->size] = hash_table_new(value_keyword_hash_code, value_keyword_equality);
    scopes->size++;
    scopes->current++;
}

void sub_vars(struct Value *v, struct Scopes *scopes, int max_depth) {
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (v->type == ARRAY) {
        for (int i = 0; i < v->data.array->size; i++) {
            sub_vars(v->data.array->values[i], scopes, max_depth);
        }
    } else if (v->type == KEYWORD) {
        hash_table *current_scope = scopes->scopes[scopes->current];
        if (hash_table_contains(current_scope, v)) {
            *v = * (struct Value *) hash_table_get(current_scope, v);
        } else {
            ERROR("Undefined variable: %s", v->data.str.body);
        }
    }
}

void printTree(struct Tree *tree){
    if(!tree->size){
      if(tree->type == 'k'){
          printf("{ type: \'%c\', size: %d, keyword: %s }", tree->type, tree->size, tree->content.data.str.body);
      }
      else if(tree->type == 'n'){
          printf("{ type: \'%c\', size: %d, value: %ld }", tree->type, tree->size, tree->content.data.ln);
      }
      else {
          printf("{ type: \'%c\', size: %d }", tree->type, tree->size);
      }
    } else {
      if(tree->type == 'k'){
          printf("{ type: \'%c\', size: %d, keyword: %s ", tree->type, tree->size, tree->content.data.str.body);
      }
      else if(tree->type == 'n'){
          printf("{ type: \'%c\', size: %d, value: %ld ", tree->type, tree->size, tree->content.data.ln);
      } else {
          printf("{ type: \'%c\', size: %d, ", tree->type, tree->size);
      }
      if(tree->size && tree->size < 100){
        printf("children: [");
        int counter = 0;
        while(counter < tree->size){
          printTree(tree->children[counter]);
          counter++;
        }
      }
      printf("]}");
    }
}
