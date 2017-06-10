#include "apply_core_function.h"
#include "constants.h"
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "config.h"
#include "execution_functions/let_binding.h"

struct Value apply_core_function(struct Value *kwd_val, struct Execution_bundle *exec_bundle __attribute((unused)), struct Value *a, struct Value *b) {
    struct String kwd = value_as_keyword(kwd_val);
    struct Value v;
    if(string_matches_heap(&kwd, &concat_const)){
        v = concat(*a, *b);
    }
    else if(string_matches_heap(&kwd, &index_const)){
        v = value_index(*a, *b);
    }
    else if(string_matches_heap(&kwd, &push_const)){
        v = push(*a, *b);
    }
    else if(string_matches_heap(&kwd, &split_const)){
        v = split(*a, *b);
    } else if (a->type == TYPE_ARRAY && b->type == TYPE_ARRAY) {
        struct Value_array *a_arr = value_as_array(a);
        struct Value_array *b_arr = value_as_array(b);
        if(a_arr->size != b_arr->size){
            ERROR("Tried to map over arrays of different sizes: %zu != %zu", a_arr->size, b_arr->size);
        }
        for(size_t i = 0; i < a_arr->size; i++){
            struct Value result = apply_core_function(kwd_val, exec_bundle, a_arr->values[i], b_arr->values[i]);
            value_copy_to(a_arr->values[i], &result);
        }
        return *a;
    } else if(string_matches_char(&kwd, '*')){
        v = mul(*a, *b);
    } else if(string_matches_char(&kwd, '+')){
        v = add(*a, *b);
    } else if(string_matches_char(&kwd, '-')){
        v = sub(*a, *b);
    } else if(string_matches_char(&kwd, '/')){
        v = divide(*a, *b);
    } else if(string_matches_char(&kwd, '!')){
        v = not(*a, *b);
    } else if(string_matches_char(&kwd, '=')){
        v = equals(*a, *b);
    } else if(string_matches_char(&kwd, '>')){
        v = greater_than(*a, *b);
    } else if(string_matches_char(&kwd, '<')){
        v = greater_than(*b, *a);
    } else if(string_matches_char(&kwd, '&')){
        v = and(*a, *b);
    } else if(string_matches_char(&kwd, '|')){
        v = or(*a, *b);
    } else {
        return *a;
    }
    *a = v;
    return v;
}
