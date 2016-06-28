#include "../structures/structures.h"
#include "../constants.h"
#include "../tokenizer.h"
#include "../parser.h"
#include "../store_defs.h"
#include "../execute.h"

void eval(struct Value * string, struct Tree_map * defined, struct Scopes * scopes, int max_depth){
    if(string->type != STRING){
        ERROR("Error passing non-string value to eval ('%c' != 's')", string->type);
    }
    struct Tokens t = {
        .length = 0,
        .counter = 0
    };
    struct Tree root = {
        .type = 'r',
        .size = 0
    };
    struct Tokens * tokens = tokenize(string->data.str, &t);
    parse(&root, tokens);
    int num_defs = store_defs(&root, defined);
    for(int i = num_defs; i < root.size; i++){
        execute(root.children[i], defined, scopes, max_depth - 1);
    }
}
