#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "structures.h"
#include "constants.h"
#include "store_defs.h"
#include "tokenizer.h"
#include "parser.h"
#include "execution_functions/read_file.h"

int store_defs(struct Tree * ast, struct Tree_map * defined){
    int num_defs = 0;
    for(int i = 0; i < ast->size; i++){
        if (ast->children[i]->type != 'k') {
            continue;
        }
        if (ast->children[i]->content.type != 'k') {
            continue;
        }
        if(string_matches(&ast->children[i]->content.data.str, &def_const)){
            if (ast->children[i]->size == 0) {
                ERROR("No function definition found");
            }
            char c = ast->children[i]->children[0]->content.type;
            if (c != 'k') {
                ERROR("Invalid type: '%c'", c);
            }
            defined->names[defined->size] = &ast->children[i]->children[0]->content.data.str;
            defined->trees[defined->size] = ast->children[i];
            defined->size++;
            num_defs++;
        }
        else if(string_matches(&ast->children[i]->content.data.str, &import_const)){
            struct Tree * imported_ast = read_import(ast->children[i]->content.data.str);
            store_defs(imported_ast, defined);
            num_defs++; // we're just skipping over an import statement here
        }
    }
    return num_defs;
}

struct Tree * read_import(struct String file_name){
    assert(string_is_sane(&file_name));
    struct Value file_contents = read_file(file_name);
    struct Tokens * tokens = malloc(sizeof(struct Tokens));
    tokens->length = 0;
    tokens->counter = 0;
    struct Tree * root = malloc(sizeof(struct Tree));
    root->type = 'r';
    root->size = 0;

    tokenize(file_contents.data.str, tokens);
    parse(root, tokens);

    free(tokens);
    return root;
}
