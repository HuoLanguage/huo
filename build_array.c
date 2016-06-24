#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "constants.h"

void build_array(struct Value_array * array, struct Tokens * tokens){
    while(tokens->tokens[tokens->counter].type != 'e'){
        tokens->counter++;
        if (tokens->counter >= tokens->length) {
            ERROR("Unbalanced array");
        }
        if(tokens->tokens[tokens->counter].type == 's'){
            struct Value * val = malloc(sizeof(struct Value));
            val->type = 's';
            val->data.str = string_copy_stack(&tokens->tokens[tokens->counter].data);
            array->values[array->size] = val;
            array->size++;
        }
        else if(tokens->tokens[tokens->counter].type == 'n'){
            struct Value * val = malloc(sizeof(struct Value));
            if(string_contains(dot_const, &tokens->tokens[tokens->counter].data)){
                float content = atof(tokens->tokens[tokens->counter].data.body);
                val->type='f';
                val->data.fl=content;
            } else {
                long content = atol(tokens->tokens[tokens->counter].data.body);
                val->type='l';
                val->data.ln=content;
            }
            array->values[array->size] = val;
            array->size++;
        }
        else if(tokens->tokens[tokens->counter].type == 'k'){
            struct Value * val = malloc(sizeof(struct Value));
            val->type = 'k';
            
            val->data.str = string_copy_stack(&tokens->tokens[tokens->counter].data);
            array->values[array->size] = val;
            array->size++;
        }
        else if(tokens->tokens[tokens->counter].type == 'b'){
            struct Value * val = malloc(sizeof(struct Value));
            val->type = 'a';
            val->data.array = malloc(sizeof(struct Value_array));
            val->data.array->size = 0;
            build_array(val->data.array, tokens);
            array->values[array->size] = val;
            array->size++;
            // to advance beyond the inner closing bracket, otherwise it
            // will cause the outer array to stop building
            tokens->counter++;
        }
    }
}
