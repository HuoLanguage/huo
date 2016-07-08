#include <stdlib.h>
#include <pthread.h>
#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

void * parallel_routine(void * bundle_ptr){
    struct Execution_bundle * bundle = (struct Execution_bundle *) bundle_ptr;
    if (bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    execute(bundle);
    return NULL;
}

#if 0
void parallel_execution(struct Execution_bundle * exec_bundle){
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    size_t num_children = ast->size;
    pthread_t *tid = ARR_MALLOC(num_children, pthread_t);
    struct Execution_bundle * bundle = ARR_MALLOC(num_children, struct Execution_bundle);
    for(size_t i = 0; i < num_children; i++){
        bundle[i].ast=exec_bundle->ast->children[i];
        bundle[i].defined=exec_bundle->defined; // BUG: defined is NOT threadsafe. Needs to be copied.
        bundle[i].scopes=exec_bundle->scopes; // BUG: scopes is NOT threadsafe. Needs to be copied.
        bundle[i].function_names=exec_bundle->function_names;
        bundle[i].max_depth = exec_bundle->max_depth - 1;
        pthread_create(&tid[i], NULL, &parallel_routine, &bundle[i]);
    }
    for (size_t j = 0; j < num_children; j++){
        pthread_join(tid[j], NULL);
    }
    free(bundle);
}
#else
// Dummy single-threaded version.
void parallel_execution(struct Execution_bundle * exec_bundle){
    if (exec_bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    struct Tree * ast = exec_bundle->ast;
    for(size_t i = 0; i < ast->size; i++){
        exec_bundle->ast = ast->children[1];
        parallel_routine(exec_bundle);
    }
}
#endif
