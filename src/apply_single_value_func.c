#include <stdio.h>
#include <stdlib.h>
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "store_defs.h"
#include "constants.h"
#include "execute.h"
#include "execution_functions/read_line.h"
#include "execution_functions/read_file.h"
#include "execution_functions/evaluate.h"

struct Value apply_single_value_func(struct Value *kwd_val, struct Execution_bundle *exec_bundle __attribute((unused)), struct Value *value) {
    struct String kwd = value_as_keyword(kwd_val);

    if(string_matches_heap(&kwd, &print_const)){
        print(*value);
        printf("\n");
        value->type = UNDEF;
        value->data.bl = false;
    } else if(string_matches_heap(&kwd, &length_const)){
        *value = value_from_long(length(*value));
    } else if(string_matches_heap(&kwd, &return_const)){
        return *value;
    } else if(string_matches_heap(&kwd, &eval_const)){
        *value = eval(value, exec_bundle);
    } else if(string_matches_heap(&kwd, &read_line_const)){
        *value = value_from_string(read_line(value));
    } else if(string_matches_heap(&kwd, &read_file_const)){
        *value = value_from_string(read_file(value_as_string(value)));
    } else if(string_matches_heap(&kwd, &import_const)){
        huo_ast *old_ast = exec_bundle->ast;
        exec_bundle->ast = read_import(value_as_string(value));
        *value = execute(exec_bundle);
        exec_bundle->ast = old_ast;
    } else if(string_concat_heap(&kwd, &typeof_const)){
        *value = value_from_string(type_to_string(value->type));
    } else {
        value->type = UNDEF;
        value->data.bl = false;
    }
    return *value;
}
