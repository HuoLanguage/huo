#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structures/structures.h"
#include "constants.h"
#include "base_util.h"

struct Value_array *build_array(struct Tokens * tokens){

    struct Value_array * array = malloc(sizeof(struct Value_array));
    array->size = 0;
    array->values = NULL;
    while (true) {
        tokens->counter += 1;
        if (tokens->counter >= tokens->length) {
            ERROR("Unbalanced array");
        }
        char c = tokens->tokens[tokens->counter].type;
        if (c == 0)
            continue;
        else if (c == 'e')
            break;

        struct Value * val = malloc(sizeof(struct Value));

        if(c == 's'){
            val->type = STRING;
            val->data.str = string_copy_stack(&tokens->tokens[tokens->counter].data);
        }
        else if(c == 'n'){
            if(string_contains(dot_const, &tokens->tokens[tokens->counter].data)){
                float f = atof(tokens->tokens[tokens->counter].data.body);
                *val = value_from_float(f);
            } else {
                long l = atol(tokens->tokens[tokens->counter].data.body);
                *val = value_from_long(l);
            }
        }
        else if(c == 'k'){
            val->type = KEYWORD;
            val->data.str = string_copy_stack(&tokens->tokens[tokens->counter].data);
        }
        else if(c == 'b'){
            val->type = ARRAY;
            val->data.array = build_array(tokens);
        } else {
            ERROR("Invalid token type in array: '%c'", c);
        }
        RESIZE(array->values, array->size+1);
        array->values[array->size] = val;
        array->size++;
    }
    return array;
}
