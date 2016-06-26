#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "structures/structures.h"
#include "base_util.h"
#include "core_functions.h"


struct Value copy_value_stack(struct Value * b){
    struct Value a;
    a.type = b->type;
    if(b->type == 's' || b->type == 'k'){
        assert(string_is_sane(&b->data.str));
        a.data.str = string_copy_stack(&b->data.str);
        assert(string_is_sane(&a.data.str));
    } else if(b->type == 'f'){
        a.data.fl = b->data.fl;
    } else if(b->type == 'l'){
        a.data.ln = b->data.ln;
    } else if (b->type == 'b'){
        a.data.bl = b->data.bl;
    } else if (b->type == 'a'){
        copy_array(&a, b->data.array);
    } else if (b->type == 'u'){
        //a.type == 'u';
    } else {
        ERROR("Unknown type: %c", a.type);
    }
    return a;
}

struct Value *copy_value_heap(struct Value * b){
    struct Value *a = malloc(sizeof(struct Value));
    if (a == NULL) {
        ERROR("Malloc failure");
    }
    a->type = b->type;
    if(b->type == 's' || b->type == 'k'){
        assert(string_is_sane(&b->data.str));
        a->data.str = string_copy_stack(&b->data.str);
        assert(string_is_sane(&a->data.str));
    } else if(b->type == 'f'){
        a->data.fl = b->data.fl;
    } else if(b->type == 'l'){
        a->data.ln = b->data.ln;
    } else if (b->type == 'b'){
        a->data.bl = b->data.bl;
    } else if (b->type == 'a'){
        copy_array(a, b->data.array);
    } else if (b->type == 'u'){
        //a->type == 'u';
    } else {
        ERROR("Unknown type: %c", a->type);
    }
    return a;
}

void copy_value_to(struct Value * a, struct Value * b){
    a->type = b->type;
    if(a->type == 's' || a->type == 'k'){
        assert(string_is_sane(&a->data.str));
        assert(string_is_sane(&b->data.str));
        string_copy_to(&a->data.str, &b->data.str);
        assert(string_is_sane(&a->data.str));
        assert(string_is_sane(&b->data.str));
    } else if(a->type == 'f'){
        a->data.fl = b->data.fl;
    } else if(a->type == 'l'){
        a->data.ln = b->data.ln;
    } else if (a->type == 'b'){
        a->data.bl = b->data.bl;
    } else if (a->type == 'a'){
        copy_array(a, b->data.array);
    } else if (b->type == 'u'){
       // a->type == 'u';
    } else {
        ERROR("Unknown type: %c", a->type);
    }
}

void copy_array(struct Value * a, struct Value_array * b){
    struct Value_array * array = malloc(sizeof(struct Value_array));
    array->size = b->size;
    for(int i = 0; i < b->size; i++){
        array->values[i] = copy_value_heap(b->values[i]);
    }
    a->data.array = array;
}

struct Tree * duplicate_tree(struct Tree * a){
    struct Tree * root = malloc(sizeof(struct Tree));
    root->type = a->type;
    root->size = 0;
    root->content = copy_value_stack(&a->content);
    for(int i = 0; i < a->size; i++){
        root->children[i] = duplicate_tree(a->children[i]);
        root->size++;
    }
    return root;
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
