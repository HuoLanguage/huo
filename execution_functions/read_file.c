#include <stdlib.h>
#include <stdio.h>
#include "../structures.h"
#include "../execute.h"

struct Value read_file(struct Tree * ast){
    struct Value result;
    FILE *fp;
    fp = fopen(ast->children[0]->content.data.str.body, "r");
    if(fp == NULL){
        printf("Error: cannot find file: \"%s\"\n", ast->children[0]->content.data.str.body);
        result.type = 'u';
        return result;
    }
    struct String file = {
        .length = 0
    };
    char c;
    while ((c = fgetc(fp)) != EOF){
        file.body[file.length] = c;
        file.length++;
    }
    fclose(fp);
    result.type = 's';
    result.data.str = file;
    return result;
}
