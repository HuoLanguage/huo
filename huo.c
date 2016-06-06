#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "parser.h"
#include "structures.h"
#include "execute.h"
#include "store_defs.h"

int main(int argc, char const *argv[]) {
    if(argc < 2){
        printf("Error: you have run Huo without pointing it to a file.\n");
        return 0;
    }

    FILE *fp;
    fp = fopen(argv[1], "r");

    struct Tokens t = {
        .length = 0,
        .counter = 0
    };
    struct String file = {
        .length = 0
    };

    char c;
    while ((c = fgetc(fp)) != EOF){
        file.body[file.length] = c;
        file.length++;
    }
    fclose(fp);

    struct Tokens *tokens = tokenize(file, &t);

    struct Tree root;
    root.type = 'r';
    root.size = 0;

    parse(&root, tokens);
    struct Tree_map * defined = malloc(sizeof(struct Tree_map));
    // this two lines print the AST for reference
    // printTree(&root);
    // printf("\n");
    int num_defs = store_defs(&root, defined);
    for(int i = num_defs; i < root.size; i++){
        execute(root.children[i], defined);
    }
    return 0;
}
