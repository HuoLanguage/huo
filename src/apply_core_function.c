#include "constants.h"
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "config.h"

struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b){
    if(ast->type == 'k'){
        struct String *kwd = &ast->content.data.str;
        if(string_matches_heap(&concat_const, kwd)){
            return concat(a, b);
        }
        else if(string_matches_heap(&index_const, kwd)){
            return index(a, b);
        }
        else if(string_matches_heap(&push_const, kwd)){
            return push(a, b);
        }
        else if(string_matches_heap(&split_const, kwd)){
            return split(a, b);
        }
    }
    else if (a.type == ARRAY && b.type == ARRAY) {
        struct Value_array *a_arr = value_as_array(&a);
        struct Value_array *b_arr = value_as_array(&b);
        if(a_arr->size != b_arr->size){
            ERROR("Tried to map over arrays of different sizes: %i != %i", a_arr->size, b_arr->size);
        }
        for(long i = 0; i < a_arr->size; i++){
            struct Value result = apply_core_function(ast, *a_arr->values[i], *b_arr->values[i]);
            value_copy_to(a_arr->values[i], &result);
        }
        return value_from_array(a_arr);
    }
    else if(ast->type == '*'){
        return mul(a, b);
    }
    else if(ast->type == '+'){
        return add(a, b);
    }
    else if(ast->type == '-'){
        return sub(a, b);
    }
    else if(ast->type == '/'){
        return divide(a, b);
    }
    else if(ast->type == '!'){
        return not(a, b);
    }
    else if(ast->type == '='){
        return equals(a, b);
    }
    else if(ast->type == '>'){
        return greater_than(a, b);
    }
    else if(ast->type == '<'){
        return greater_than(b, a);
    }
    return a;
}
