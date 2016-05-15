#include "structures.h"
#include "core_functions.c"

struct Value execute (struct Tree *);

struct Value reduce(struct Tree * children[], int length, char func){

    struct Value result = execute(children[0]);

    for(int i = 1; i < length; i++){
        if(children[i]->type == 'f'){

            struct Value newResult = execute(children[i]);

            if(func == '*'){
                result = mul(result, newResult);
            }
            else if(func == '+'){
                result = add(result, newResult);
            }
            else if(func == '-'){
                result = sub(result, newResult);
            }
        } else {
            if(func == '*'){
                result = mul(result, children[i]->content);
            }
            else if(func == '+'){
                result = add(result, children[i]->content);
            }
            else if(func == '-'){
                result = sub(result, children[i]->content);
            }
        }
    }
    return result;
}

struct Value execute (struct Tree * ast){
    struct Value result;
    if(!ast->size){
        return ast->content;
    }
    else if(ast->size > 2){
        result = reduce(ast->children, ast->size, ast->type);
    }
    else if(ast->size == 1){
        struct Value a = execute(ast->children[0]);
        if(ast->type == 'k'){
            if(string_matches(ast->content.data.str, print_const)){
                print(a);
            }
        }
    } else {
        struct Value a = execute(ast->children[0]);
        struct Value b = execute(ast->children[1]);

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
    }
    return result;
}
