#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structures/structures.h"
#include "constants.h"
#include "base_util.h"
#include "config.h"

struct Value_array *build_array(struct Tokens * tokens){

    struct Value_array * array = malloc_or_die(sizeof(struct Value_array));
    array->size = 0;
    array->values = NULL;
    while (true) {
        tokens->counter += 1;
        if (tokens->counter >= tokens->length) {
            ERROR("Unbalanced array");
        }

        struct Value val;
        char c = tokens->tokens[tokens->counter].type;
        if (c == 0) {
            continue;
        }
        else if (c == 'e') {
            break;
        }
        else if (c == 's'){
            val = value_from_string(tokens->tokens[tokens->counter].data);
        }
        else if(c == 'n'){
            if(string_contains(dot_const, &tokens->tokens[tokens->counter].data)){
                float f = atof(tokens->tokens[tokens->counter].data.body);
                val = value_from_float(f);
            } else {
                long l = atol(tokens->tokens[tokens->counter].data.body);
                val = value_from_long(l);
            }
        }
        else if(c == 'k'){
            val = value_from_keyword(&tokens->tokens[tokens->counter].data);
        }
        else if(c == 'b'){
            val = value_from_array(build_array(tokens));
        } else {
            ERROR("Invalid token type in array: '%c'", c);
        }
        RESIZE(array->values, array->size+1);
        array->values[array->size] = value_copy_heap(&val);
        array->size++;
    }
    return array;
}
