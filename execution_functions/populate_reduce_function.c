#include "../structures.h"
#include "../base_util.h"

void populate_reduce_function(
  struct Value * accumulator_key,
  struct Value * current_key,
  struct Tree * ast,
  struct Value * item,
  struct Value * accumulator){
    if(!ast->size && ast->type == 'k'){
        if(string_matches(&current_key->data.str, &ast->content.data.str)){
            if(item->type == 's'){
              ast->type = item->type;
            } else {
              ast->type = 'n';
            }
            copy_value(&ast->content, item);
        }
        else if(string_matches(&accumulator_key->data.str, &ast->content.data.str)){
            if(accumulator->type == 's'){
              ast->type = accumulator->type;
            } else {
              ast->type = 'n';
            }
            copy_value(&ast->content, accumulator);
        }
    } else if(ast->size > 0){
        for(int i = 0; i < ast->size; i++){
            populate_reduce_function(accumulator_key, current_key, ast->children[i], item, accumulator);
        }
    }
}
