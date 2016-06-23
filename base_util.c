#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "base_util.h"
#include "core_functions.h"

int string_contains(char ch, struct String* string){
    if(!string->length){ return 0; }
    for(int i = 0; i < string->length; i++){
        if(ch == string->body[i]){
            return 1;
        }
    }
    return 0;
}

void copy_value(struct Value * a, struct Value * b){
    a->type = b->type;
    if(a->type == 's' || a->type == 'k'){
        a->data.str.length = b->data.str.length;
        strcpy(a->data.str.body, b->data.str.body);
    } else if(a->type == 'f'){
        a->data.fl = b->data.fl;
    } else if(a->type == 'l'){
        a->data.ln = b->data.ln;
    } else if (a->type == 'b'){
        a->data.bl = b->data.bl;
    } else if (a->type == 'a'){
        copy_array(a, b->data.array);
    }
}

void copy_array(struct Value * a, struct Value_array * b){
    struct Value_array * array = malloc(sizeof(struct Value_array));
    array->size = b->size;
    for(int i = 0; i < b->size; i++){
        struct Value * new_item = malloc(sizeof(struct Value));
        copy_value(new_item, b->values[i]);
        array->values[i] = new_item;
    }
    a->data.array = array;
}

struct Tree * duplicate_tree(struct Tree * a){
    struct Tree * root = malloc(sizeof(struct Tree));
    root->type = a->type;
    root->size = 0;
    copy_value(&root->content, &a->content);
    for(int i = 0; i < a->size; i++){
        root->children[i] = duplicate_tree(a->children[i]);
        root->size++;
    }
    return root;
}

int string_matches(struct String base, struct String compare){
    if(base.length != compare.length){
        return 0;
    }
    int counter = 0;
    while(counter < base.length){
        if(base.body[counter] != compare.body[counter]){
            return 0;
        }
        counter++;
    }
    return 1;
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
