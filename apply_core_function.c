#include "constants.h"
#include "structures.h"
#include "core_functions.h"
#include "base_util.h"

struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b){
    struct Value result;

    if(ast->type == 'k'){
        if(string_matches(ast->content.data.str, concat_const)){
            result = concat(a, b);
        }
        else if(string_matches(ast->content.data.str, index_const)){
            result = array_index(a, b);
        }
        else if(string_matches(ast->content.data.str, push_const)){
            result = array_push(a, b);
        }
        else if(string_matches(split_const, ast->content.data.str)){
            result = split_string(a, b);
        }
    }
    else if(ast->type == '*'){
        result = mul(a, b);
    }
    else if(ast->type == '+'){
        result = add(a, b);
    }
    else if(ast->type == '-'){
        result = sub(a, b);
    }
    else if(ast->type == '/'){
        result = divide(a, b);
    }
    else if(ast->type == '!'){
        result = not(a, b);
    }
    else if(ast->type == '='){
        result = equals(a, b);
    }
    else if(ast->type == '>'){
        result = greater_than(a, b);
    }
    else if(ast->type == '<'){
        result = greater_than(b, a);
    }
    return result;
}
