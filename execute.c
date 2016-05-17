#include "structures.h"
#include "core_functions.c"

struct Value execute (struct Tree *);
struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b);

struct Value reduce(struct Tree * ast){
    struct Value result = execute(ast->children[0]);

    for(int i = 1; i < ast->size; i++){
        if(ast->children[i]->type == 'f'){
            struct Value newResult = execute(ast->children[i]);
            result = apply_core_function(ast, result, newResult);
        } else {
            result = apply_core_function(ast, result, ast->children[i]->content);
        }
    }
    return result;
}

struct Value execute (struct Tree * ast){
    struct Value result;
    // first check for special kinds of execution
    if(string_matches(ast->content.data.str, if_const)){
        result = execute(ast->children[0]);
        if(result.type != 'b'){
            // throw error
        }
        else if(result.data.bl == 't'){
            result = execute(ast->children[1]);
        }
        else if(result.data.bl == 'f'){
            result = execute(ast->children[2]);
        }
        return result;
    }
    
    // then check for function invocations
    if(!ast->size){
        return ast->content;
    }
    else if(ast->size > 2){
        result = reduce(ast);
    }
    else if(ast->size == 1){
        struct Value a = execute(ast->children[0]);
        if(ast->type == 'k'){
            if(string_matches(ast->content.data.str, print_const)){
                print(a);
                printf("\n");
            }
        }
    } else {
        struct Value a = execute(ast->children[0]);
        struct Value b = execute(ast->children[1]);
        result = apply_core_function(ast, a, b);
    }
    return result;
}

struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b){
    struct Value result;

    if(ast->type == 'k'){
        if(string_matches(ast->content.data.str, concat_const)){
            result = concat(a, b);
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
    return result;
}
