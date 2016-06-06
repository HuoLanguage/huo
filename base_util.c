#include "structures.h"
#include <string.h>
#include <stdio.h>

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
    if(a->type == 's'){
        a->data.str.length = b->data.str.length;
        strcpy(a->data.str.body, b->data.str.body);
    } else if(a->type == 'f'){
        a->data.fl = b->data.fl;
    } else if(a->type == 'l'){
        a->data.ln = b->data.ln;
    } else if (a->type == 'b'){
        a->data.bl = b->data.bl;
    }
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
      printf("{ type: \'%c\', size: %d }", tree->type, tree->size);
    } else {
      printf("{ type: \'%c\', size: %d, ", tree->type, tree->size);
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
