#include <stdlib.h>
#include <pthread.h>
#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"

void * parallel_routine(void * bundle_ptr){
    struct Execution_bundle * bundle = (struct Execution_bundle *) bundle_ptr;
    if (bundle->max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    execute(bundle->ast, bundle->defined, bundle->scopes, bundle->max_depth - 1);
    return 0;
}

void parallel_execution(struct Tree * ast, struct Tree_map * defined, struct Scopes * scopes, int max_depth){
    int num_children = ast->size;
    pthread_t tid[num_children];
    struct Execution_bundle * bundle[num_children];
    for(int i = 0; i < num_children; i++){
        bundle[i] = malloc(sizeof(struct Execution_bundle));
        bundle[i]->ast=ast->children[i];
        bundle[i]->defined=defined;
        bundle[i]->scopes=scopes;
        bundle[i]->max_depth = max_depth - 1;
        pthread_create(&tid[i], NULL, &parallel_routine, bundle[i]);
    }
    for (int j = 0; j < num_children; j++){
        pthread_join(tid[j], NULL);
        free(bundle[j]);
    }
}
