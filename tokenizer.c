#include <string.h>
#include <stddef.h>
#include "constants.h"

struct Tokens * tokenize(struct String file, struct Tokens *content){
    uint8_t counter = 0;
    while (counter < file.length){
        char c = file.body[counter];
        if(c != ' ' && c != '\n'){
            struct Token t = {
                .data = {
                    .length = 0
                }
            };
            if(is_a_open_parens(c)){
                t.type = 'o';
            }
            else if(is_a_close_parens(c)){
                t.type = 'c';
            }
            else if(is_a_number(c)){
                t.type = 'n';
                while(is_a_number(c)){
                    t.data.body[t.data.length] = file.body[counter];
                    t.data.length++;
                    counter++;
                    c = file.body[counter];
                }
                counter--;
            }
            else if(is_a_function(c)){
                t.type = c;
            }
            content->tokens[content->length] = t;
            content->length++;
        }
        counter++;
    }

    return content;
}
