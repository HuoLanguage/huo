#include "../structures.h"
#include "../execute.h"

struct Value if_block(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    struct Value result = execute(ast->children[0], defined, let_map);
    if(result.type != 'b'){ // throw error
    }
    else if(result.data.bl == 't'){ // result is boolean true
        result = execute(ast->children[1], defined, let_map);
    }
    else if(result.data.bl == 'f'){ // result is boolean false
        result = execute(ast->children[2], defined, let_map);
    }
    return result;
}
