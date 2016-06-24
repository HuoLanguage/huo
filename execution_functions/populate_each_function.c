#include "../structures.h"
#include "../base_util.h"

void populate_each_function(
  struct Value * item_key,
  struct Value * index_key,
  struct Tree * ast,
  struct Value * item,
  struct Value * index){
    if(!ast->size && ast->type == 'k'){
        if(string_matches(item_key->data.str, ast->content.data.str)){
            if(item->type == 's'){
              ast->type = item->type;
            } else {
              ast->type = 'n';
            }
            copy_value(&ast->content, item);
        }
        else if(string_matches(index_key->data.str, ast->content.data.str)){
            ast->type = 'n';
            copy_value(&ast->content, index);
        }
    } else if(ast->size > 0){
        for(int i = 0; i < ast->size; i++){
            populate_each_function(item_key, index_key, ast->children[i], item, index);
        }
    }
}
