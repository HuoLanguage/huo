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
    execute(bundle->ast, bundle->defined, bundle->scopes, bundle->max_depth - 1);
    return NULL;
}

#if 0
void parallel_execution(struct Tree * ast, hash_table *defined, struct Scopes * scopes, huo_depth_t max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    size_t num_children = ast->size;
    pthread_t *tid = ARR_MALLOC(num_children, pthread_t);
    struct Execution_bundle * bundle = ARR_MALLOC(num_children, struct Execution_bundle);
    for(size_t i = 0; i < num_children; i++){
        bundle[i].ast=ast->children[i];
        bundle[i].defined=defined; // BUG: defined is NOT threadsafe. Needs to be copied.
        bundle[i].scopes=scopes; // BUG: scopes is NOT threadsafe. Needs to be copied.
        bundle[i].max_depth = max_depth - 1;
        pthread_create(&tid[i], NULL, &parallel_routine, &bundle[i]);
    }
    for (size_t j = 0; j < num_children; j++){
        pthread_join(tid[j], NULL);
    }
    free(bundle);
}
#else
// Dummy single-threaded version.
void parallel_execution(struct Tree * ast, hash_table *defined, struct Scopes * scopes, huo_depth_t max_depth){
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    for(size_t i = 0; i < ast->size; i++){
        struct Execution_bundle bundle = {
            .ast=ast->children[i],
            .defined=defined,
            .scopes=scopes,
            .max_depth = max_depth - 1
        };
        parallel_routine(&bundle);
    }
}
#endif
