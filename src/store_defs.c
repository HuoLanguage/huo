#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "structures/structures.h"
#include "constants.h"
#include "store_defs.h"
#include "tokenizer.h"
#include "parser.h"
#include "execution_functions/read_file.h"
#include "structures/hash_table.h"
#include "config.h"

huo_ast * read_import(struct String file_name){
    assert(string_is_sane(&file_name));
    struct String file_contents = read_file(file_name);
    struct Tokens * tokens = malloc_or_die(sizeof(struct Tokens));
    tokens->tokens = NULL;
    tokens->length = 0;
    tokens->counter = 0;

    tokenize(file_contents, tokens);
    huo_ast *root = parse(tokens);

    free(tokens);
    return root;
}
