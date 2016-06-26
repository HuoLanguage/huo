#include "../structures/structures.h"
#include "../base_util.h"

void populate_each_function(
  struct Value * item_key,
  struct Value * index_key,
  struct Tree * ast,
  struct Value * item,
  struct Value * index){
    if (item_key->type != KEYWORD) {
        ERROR("Invalid item key type: '%c'", index_key->type);
    }
    if (index_key->type != KEYWORD) {
        ERROR("Invalid index key type: '%c'", index_key->type);
    }
    if(!ast->size && ast->type == 'k' && ast->content.type == KEYWORD){
        if(string_matches(&item_key->data.str, &ast->content.data.str)){
            if(item->type == 's'){
              ast->type = item->type;
            } else {
              ast->type = 'n';
            }
            ast->content = copy_value_stack(item);
        }
        else if(string_matches(&index_key->data.str, &ast->content.data.str)){
            ast->type = 'n';
            ast->content = copy_value_stack(index);
        }
    } else if(ast->size > 0){
        for(int i = 0; i < ast->size; i++){
            populate_each_function(item_key, index_key, ast->children[i], item, index);
        }
    }
}
