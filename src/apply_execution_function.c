#include <stdio.h>
#include <stdlib.h>
#include "structures/structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "store_defs.h"
#include "constants.h"
#include "execute.h"
#include "execution_functions/for_each.h"
#include "execution_functions/for_loop.h"
#include "execution_functions/reduce.h"
#include "execution_functions/read_file.h"
#include "execution_functions/map_array.h"
#include "execution_functions/if_block.h"
#include "execution_functions/let_binding.h"
#include "execution_functions/switch.h"
#include "execution_functions/parallel_execution.h"
#include "execution_functions/while_loop.h"


bool apply_execution_function(struct Value *kwd_val, struct Value *result, struct Execution_bundle * exec_bundle){
    huo_ast * ast = exec_bundle->ast;


    //printf("AEF: %s\n", string_to_chars(ast_to_string(ast)));

    struct Value undefined = value_from_undef();

    struct String kwd = value_as_keyword(kwd_val);

    if(string_matches_heap(&kwd, &if_const)){
        *result = if_block(exec_bundle);
        return true;
    }
    else if(string_matches_heap(&kwd, &each_const)){
        for_each(exec_bundle);
        *result = undefined;
        return true;
    }
    else if(string_matches_heap(&kwd, &map_const)){
        *result = map_array(exec_bundle);
        return true;
    }
    else if(string_matches_heap(&kwd, &while_const)){
        while_loop(exec_bundle);
        *result = undefined;
        return true;
    }
    else if(string_matches_heap(&kwd, &reduce_const)){
        *result = reduce_array(exec_bundle);
        return true;
    }
    else if(string_matches_heap(&kwd, &set_const)) {
        if (ast_size(ast) != 4) {
            ERROR("Not enough arguments for set: %zu < 4", ast_size(ast));
        }
        exec_bundle->ast = ast_child(ast, 1);
        struct Value index = execute(exec_bundle);

        exec_bundle->ast = ast_child(ast, 2);
        struct Value item = execute(exec_bundle);

        exec_bundle->ast = ast_child(ast, 3);
        struct Value array = execute(exec_bundle);

        exec_bundle->ast = ast;

        set(index, item, &array);
        *result = array;
        return true;
    }
    else if(string_matches_heap(&kwd, &for_const)){
        for_loop(exec_bundle);
        *result = undefined;
        return true;
    }
    else if(string_matches_heap(&kwd, &do_const)){
        if (ast_size(ast) <= 1) {
            ERROR("Not enough arguments for do: %zu < 1", ast_size(ast));
        }
        for(size_t i = 1; i < ast_size(ast); i++){
            exec_bundle->ast = ast_child(ast, i);
            *result = execute(exec_bundle);
        }
        exec_bundle->ast = ast;
        return true;
    }
    else if(string_matches_heap(&kwd, &substring_const)){
        if (ast_size(ast) != 4) {
            ERROR("Not enough arguments for substring: %zu < 4", ast_size(ast));
        }

        exec_bundle->ast = ast_child(ast, 1);
        struct Value start = execute(exec_bundle);
        exec_bundle->ast = ast_child(ast, 2);
        struct Value end = execute(exec_bundle);
        exec_bundle->ast = ast_child(ast, 3);
        struct Value string = execute(exec_bundle);

        exec_bundle->ast = ast;

        *result = substring(start, end, string);
        return true;
    }
    else if(string_matches_heap(&kwd, &switch_const)){
        *result = switch_case(exec_bundle);
        return true;
    }
    else if(string_matches_heap(&kwd, &parallel_const)){
        parallel_execution(exec_bundle);
        *result = undefined;
        return true;
    }
    else if(string_matches_heap(&kwd, &let_const)){
        struct Value *name = ast_value(ast_child(ast, 1));

        exec_bundle->ast = ast_copy(ast_child(ast, 2));
        struct Value val = execute(exec_bundle);

        exec_bundle->ast = ast;

        store_let_value(name, &val, exec_bundle->scopes);


        *result = value_from_undef();
        return true;
    }
    else if(string_matches_heap(&kwd, &def_const)){

        struct Value *name = ast_value(ast_child(ast, 1));

        store_def_func(name, ast, exec_bundle->scopes);
        *result = value_from_undef();
        return true;
    }
    return false;
}
