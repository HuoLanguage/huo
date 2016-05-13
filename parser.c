#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structures.h"

struct Tree * parse(struct Tree * root, struct Tokens *tokens){

    while(tokens->counter < tokens->length){
        struct Token token = tokens->tokens[tokens->counter];
        if(token.type == 'o'){
            struct Tree * tree = malloc(sizeof(struct Tree));
            tree->type = 'f';
            tree->size = 0;

            tokens->counter++;
            struct Tree * child = parse(tree, tokens);
            root->children[root->size] = child;
            root->size++;
        }
        else if(is_a_function(token.type)){
            struct String content = {
                .length = 1
            };
            root->content = content;
            root->content.body[0] = token.type;
        }
        else if(token.type == 'n'){
            struct Tree * value = malloc(sizeof(struct Tree));
            value->type = 'n';
            value->size = 0;
            struct String content = {
                .length = token.data.length
            };
            value->content = content;
            strcpy(value->content.body, token.data.body);

            root->children[root->size] = value;
            root->size++;
        }
        else if(token.type == 'c'){
            return root;
        }
        tokens->counter++;
    }
    return root;
}
