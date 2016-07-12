#include "../structures/structures.h"
#include "../constants.h"
#include "../tokenizer.h"
#include "../parser.h"
#include "../store_defs.h"
#include "../execute.h"
#include "../core_functions.h"
#include "../config.h"

struct Value eval(struct Value *string, struct Execution_bundle * exec_bundle) {

    struct Value result;
    struct Tokens t = {
        .length = 0,
        .counter = 0
    };
    struct Tokens * tokens = tokenize(value_as_string(string), &t);
    huo_ast *root = parse(tokens);
    huo_ast *ast = exec_bundle->ast;
    for(size_t i = 0; i < ast_size(root); i++) {
        exec_bundle->ast = ast_child(root, i);
        result = execute(exec_bundle);
    }
    exec_bundle->ast = ast;
    return result;
}
