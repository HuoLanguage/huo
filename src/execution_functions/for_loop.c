#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

void for_loop(struct Execution_bundle *exec_bundle){
    struct Tree * ast = exec_bundle->ast;
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (ast->size != 3) {
        ERROR("Wrong number of arguments for for_loop: %zu != 3\n", ast->size);
    }
    exec_bundle->ast = ast->children[0];
    struct Value start = execute(exec_bundle);
    exec_bundle->ast = ast->children[1];
    struct Value end = execute(exec_bundle);

    huo_int_t start_i = value_as_long(&start);
    huo_int_t end_i = value_as_long(&end);

    if(start_i > end_i){
        for(huo_int_t i = start_i; i > end_i; i--){

            exec_bundle->ast = ast->children[2];
            execute(exec_bundle);
        }
    } else {
        for(huo_int_t i = start_i; i < end_i; i++){

            exec_bundle->ast = ast->children[2];
            execute(exec_bundle);
        }
    }
}
