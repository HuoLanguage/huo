#include <stdlib.h>
#include <string.h>
#include "structures/structures.h"
#include "constants.h"
#include "base_util.h"

void build_array(struct Value_array * array, struct Tokens * tokens){
    char c;
    while ((c = tokens->tokens[++tokens->counter].type) != 'e'){
        if (tokens->counter >= tokens->length) {
            ERROR("Unbalanced array");
        }
        if(c == 's'){
            struct Value * val = malloc(sizeof(struct Value));
            val->type = 's';
            val->data.str = string_copy_stack(&tokens->tokens[tokens->counter].data);
            array->values[array->size] = val;
            array->size++;
        }
        else if(c == 'n'){
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
        else if(c == 'k'){
            struct Value * val = malloc(sizeof(struct Value));
            val->type = 'k';
            
            val->data.str = string_copy_stack(&tokens->tokens[tokens->counter].data);
            array->values[array->size] = val;
            array->size++;
        }
        else if(c == 'b'){
            struct Value * val = malloc(sizeof(struct Value));
            val->type = 'a';
            val->data.array = malloc(sizeof(struct Value_array));
            val->data.array->size = 0;
            build_array(val->data.array, tokens);
            array->values[array->size] = val;
            array->size++;
        } else if (c == 0) {
            //pass
        } else {
            for (int i = 0; i <= tokens->counter; i++) {
                struct String s = tokens->tokens[i].data;
                printf("%i \"", i);
                printf(s.length == 0 ? "" : s.body);
                printf("\" '%c' '%i'\n", tokens->tokens[i].type, (int) tokens->tokens[i].type);
            }
            ERROR("Invalid token type in array: '%c'", c);
        }
    }
}
