#include <stdlib.h>
#include <stdio.h>
#include "../structures.h"
#include "../execute.h"

struct Value read_file(struct String file_name){
    struct Value result;
    FILE *fp;
    fp = fopen(file_name.body, "r");
    if(fp == NULL){
        printf("Error: cannot find file: \"%s\"\n", file_name.body);
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
