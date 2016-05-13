#include "structures.h"
#include "core_functions.c"

long execute (struct Tree *);

long reduce(struct Tree * children[], int length, char func){

    long result = execute(children[0]);

    for(int i = 1; i < length; i++){
        if(children[i]->type == 'f'){

            long newResult = execute(children[i]);

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
                result = mul(result, atol(children[i]->content.body));
            }
            else if(func == '+'){
                result = add(result, atol(children[i]->content.body));
            }
            else if(func == '-'){
                result = sub(result, atol(children[i]->content.body));
            }
        }
    }
    return result;
}

long execute (struct Tree * ast){
    long result = 0;
    if(!ast->size){
        return atol(ast->content.body);
    }
    else if(ast->size > 2){
        result = reduce(ast->children, ast->size, ast->content.body[0]);
    } else {
        long a = 0;
        long b = 0;
        if(ast->children[0]->type == 'f'){
            a = execute(ast->children[0]);
        } else {
            a = atol(ast->children[0]->content.body);
        }

        if(ast->children[1]->type == 'f'){
            b = execute(ast->children[1]);
        } else {
            b = atol(ast->children[1]->content.body);
        }

        if(ast->content.body[0] == '*'){
            result = mul(a, b);
        }
        else if(ast->content.body[0] == '+'){
            result = add(a, b);
        }
        else if(ast->content.body[0] == '-'){
            result = sub(a, b);
        }
        else if(ast->content.body[0] == '/'){
            result = divide(a, b);
        }
    }
    return result;
}
