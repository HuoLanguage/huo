#include <stdint.h>
#include <stdio.h>
#include "tokenizer.c"
#include "parser.c"
#include "structures.h"
#include "execute.c"
//
void printTree(struct Tree *tree){
    printf("{ type: %c, size: %d ", tree->type, tree->size);
    if(tree->size && tree->size < 100){
        printf("children: ");
        int counter = 0;
        while(counter < tree->size){
            printTree(tree->children[counter]);
            counter++;
        }
    }
    printf("}");
};

int main(int argc, char const *argv[]) {
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
    struct String content = {
        .body = "root",
        .length = 4
    };
    root.content = content;

    struct Tree * ast = parse(&root, tokens);
    // printTree(ast); this two lines print the AST for reference
    // printf("\n");
    long result = execute(ast->children[0]);
    printf("%ld", result);
    return 0;
}
