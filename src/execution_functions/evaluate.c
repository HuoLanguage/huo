#include "../structures/structures.h"
#include "../constants.h"
#include "../tokenizer.h"
#include "../parser.h"
#include "../store_defs.h"
#include "../execute.h"
#include "../core_functions.h"
#include "../config.h"

struct Value eval(struct Value * string, struct Execution_bundle * exec_bundle){
    struct Value result;
    struct Tokens t = {
        .length = 0,
        .counter = 0
    };
    struct Tree root = {
        .type = 'r',
        .size = 0
    };
    struct Tokens * tokens = tokenize(value_as_string(string), &t);
    parse(&root, tokens, true);
    size_t num_defs = store_defs(&root, exec_bundle->defined);
    for(size_t i = num_defs; i < root.size; i++){
        exec_bundle->ast = root.children[i];
        result = execute(exec_bundle);
    }
    return result;
}
