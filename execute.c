#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "core_functions.h"
#include "base_util.h"
#include "process_defs.h"
#include "constants.h"
#include "execute.h"
#include "populate_each_function.h"

struct Value reduce(struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    struct Value result = execute(ast->children[0], defined, let_map);

    for(int i = 1; i < ast->size; i++){
        if(ast->children[i]->type == 'f'){
            struct Value newResult = execute(ast->children[i], defined, let_map);
            result = apply_core_function(ast, result, newResult);
        } else {
            result = apply_core_function(ast, result, ast->children[i]->content);
        }
    }
    return result;
}

struct Value execute (struct Tree * ast, struct Tree_map * defined, struct Map * let_map){
    struct Value result;
    // first check for special kinds of execution
    if(string_matches(ast->content.data.str, if_const)){
        result = execute(ast->children[0], defined, let_map);
        if(result.type != 'b'){ // throw error
        }
        else if(result.data.bl == 't'){ // result is boolean true
            result = execute(ast->children[1], defined, let_map);
        }
        else if(result.data.bl == 'f'){ // result is boolean false
            result = execute(ast->children[2], defined, let_map);
        }
        return result;
    }
    if(string_matches(let_const, ast->content.data.str)){
      struct Keyval * let_binding = malloc(sizeof(struct Keyval));
      let_binding->key = malloc(sizeof(struct Value));
      copy_value(let_binding->key, &ast->children[0]->content);
      struct Value val = execute(ast->children[1], defined, let_map);
      struct Value * val_ptr = malloc(sizeof(struct Value));
      copy_value(val_ptr, &val);
      let_binding->val = val_ptr;
      int index = -1;
      for(int i = 0; i < let_map->size; i++){
          if(string_matches(let_binding->key->data.str, let_map->members[i]->key->data.str)){
              index = i;
          }
      }
      if(index > -1){
          let_map->members[index] = let_binding;
      } else {
          let_map->members[let_map->size] = let_binding;
          let_map->size++;
      }
      return val;
    }
    if(string_matches(each_const, ast->content.data.str)){
        struct Value array = execute(ast->children[0], defined, let_map);
        for(int i = 0; i < array.data.array->size; i++){
            struct Value * item = array.data.array->values[i];
            struct Value index = {
                .type = 'l',
                .data = {
                    .ln=(long)i
                }
            };
            struct Tree * function = duplicate_tree(ast->children[3]);
            populate_each_function(&ast->children[1]->content, &ast->children[2]->content, function, item, &index);
            execute(function, defined, let_map);
        }
        return array;
    }
    if(string_matches(map_const, ast->content.data.str)){
        struct Value array = execute(ast->children[0], defined, let_map);
        for(int i = 0; i < array.data.array->size; i++){
            struct Value * item = array.data.array->values[i];
            struct Value index = {
                .type = 'l',
                .data = {
                    .ln=(long)i
                }
            };
            struct Tree * function = duplicate_tree(ast->children[3]);
            populate_each_function(&ast->children[1]->content, &ast->children[2]->content, function, item, &index);
            struct Value result = execute(function, defined, let_map);
            copy_value(item, &result);
        }
        return array;
    }

    int idx;
    if(!ast->size){
        if(ast->type == 'k'){
            for(int i = 0; i < let_map->size; i++){
              if(string_matches(let_map->members[i]->key->data.str, ast->content.data.str)){
                  return *let_map->members[i]->val;
              }
            }
        } else {
            return ast->content;
        }
    }
    else if(ast->type == 'k' && (idx = is_defined_func(defined, ast->content.data.str)) > -1){
        return execute_defined_func(ast, defined, let_map, idx);
    }
    else if(ast->size == 1){
        struct Value a = execute(ast->children[0], defined, let_map);
        if(ast->type == 'k'){
            if(string_matches(ast->content.data.str, print_const)){
                print(a);
                printf("\n");
            }
            if(string_matches(ast->content.data.str, length_const)){
                return length(a);
            }
        }
    }
    else if(ast->size == 2) {
        struct Value a = execute(ast->children[0], defined, let_map);
        struct Value b = execute(ast->children[1], defined, let_map);
        result = apply_core_function(ast, a, b);
    } else {
        result = reduce(ast, defined, let_map);
    }
    return result;
}

struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b){
    struct Value result;

    if(ast->type == 'k'){
        if(string_matches(ast->content.data.str, concat_const)){
            result = concat(a, b);
        }
        else if(string_matches(ast->content.data.str, index_const)){
            result = array_index(a, b);
        }
        else if(string_matches(ast->content.data.str,push_const)){
            result = array_push(a, b);
        }
    }
    else if(ast->type == '*'){
        result = mul(a, b);
    }
    else if(ast->type == '+'){
        result = add(a, b);
    }
    else if(ast->type == '-'){
        result = sub(a, b);
    }
    else if(ast->type == '/'){
        result = divide(a, b);
    }
    else if(ast->type == '!'){
        result = not(a, b);
    }
    else if(ast->type == '='){
        result = equals(a, b);
    }
    return result;
}

struct Value execute_defined_func(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int idx){
    struct Map * arguments = make_args_map(ast, defined, idx);
    struct Tree * populated_ast = populate_args(arguments, get_defined_body(defined->trees[idx]));
    return execute(populated_ast, defined, let_map);
}
