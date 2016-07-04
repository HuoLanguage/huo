#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>
#include "structures/structures.h"
#include "base_util.h"
#include "core_functions.h"
#include "config.h"
#include "config.h"

bool __size_t_mul_overflow(size_t a, size_t b, size_t *res) {
#if defined(MUL_OVERFLOW)
    return MUL_OVERFLOW(a, b, res);
#elif SIZE_MAX <= (UINTMAX_MAX / SIZE_MAX)
    uintmax_t res_long = (uintmax_t) a * (uintmax_t) b;
    if (res_long > SIZE_MAX)
        return true;
    *res = (size_t) res_long;
    return false;
#elif defined(__UINT128__)
    if (sizeof(__UINT128__) >= sizeof(size_t) * 2) {
        __UINT128__ res_long = (__UINT128__) a * (__UINT128__) b;
        if (res_long > SIZE_MAX)
            return true;
        *res = (size_t) res_long;
        return false;
    } else {
        WARN_ONCE("size_t is HOW long?");
        WARN_ONCE("Using slow but portable overflow test")
    }
#else
#pragma message ("Using slow but portable overflow test")
#endif
    //Slow but portable
    size_t res_short = a * b;
    if ((a != 0) && ((res_short / a) != b))
        return true;
    *res = res_short;
    return false;
}

void *malloc_or_die(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL)
        ERROR("Malloc error");
    return ptr;
}

size_t arr_malloc_size(size_t num, size_t size) {
    size_t res;
    if (__size_t_mul_overflow(num, size, &res)) {
        ERROR("Overflow in array allocation!");
    }
    return res;
}

char *strdup(const char *str) {
    size_t len = strlen(str);
    char *dup = ARR_MALLOC(len + 1, char);
    strcpy(dup, str);
    return dup;
}

struct Tree * duplicate_tree(struct Tree * a){
    struct Tree * root = malloc_or_die(sizeof(struct Tree));
    root->type = a->type;
    root->content = value_copy_stack(&a->content);
    root->children = NULL;
    root->size = a->size;
    RESIZE(root->children, root->size);
    for(int i = 0; i < a->size; i++){
        root->children[i] = duplicate_tree(a->children[i]);
    }
    return root;
}

void make_scope(struct Scopes * scopes){
    RESIZE(scopes->scopes, scopes->size + 1);
    scopes->scopes[scopes->size] = hash_table_new(value_keyword_hash_code, value_keyword_equality);
    scopes->size++;
    scopes->current++;
}

void sub_vars(struct Value *v, struct Scopes *scopes, int max_depth) {
    if (max_depth <= 0) {
        ERROR("Max depth exceeded in computation");
    }
    if (v->type == ARRAY) {
        for (int i = 0; i < v->data.array->size; i++) {
            sub_vars(v->data.array->values[i], scopes, max_depth);
        }
    } else if (v->type == KEYWORD) {
        hash_table *current_scope = scopes->scopes[scopes->current];
        if (hash_table_contains(current_scope, v)) {
            *v = * (struct Value *) hash_table_get(current_scope, v);
        } else {
            struct String kwd = value_as_string(v);
            ERROR("Undefined variable: %s", string_to_chars(&kwd));
        }
    }
}

void printTree(struct Tree *tree){
    if(!tree->size){
      if(tree->type == 'k'){
          printf("{ type: \'%c\', size: %d, keyword: %s }", tree->type, tree->size, tree->content.data.str.body);
      }
      else if(tree->type == 'n'){
          printf("{ type: \'%c\', size: %d, value: %ld }", tree->type, tree->size, tree->content.data.ln);
      }
      else {
          printf("{ type: \'%c\', size: %d }", tree->type, tree->size);
      }
    } else {
      if(tree->type == 'k'){
          printf("{ type: \'%c\', size: %d, keyword: %s ", tree->type, tree->size, tree->content.data.str.body);
      }
      else if(tree->type == 'n'){
          printf("{ type: \'%c\', size: %d, value: %ld ", tree->type, tree->size, tree->content.data.ln);
      } else {
          printf("{ type: \'%c\', size: %d, ", tree->type, tree->size);
      }
      if(tree->size && tree->size < 100){
        printf("children: [");
        int counter = 0;
        while(counter < tree->size){
          printTree(tree->children[counter]);
          counter++;
        }
      }
      printf("]}");
    }
}
