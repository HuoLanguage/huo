#include <string.h>
#include <stddef.h>
#include "constants.h"

struct Tokens * tokenize(struct String file, struct Tokens *content){
    int counter = 0;
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
            else if(is_a_bracket(c)){
                t.type = 'b';
            }
            else if(is_a_end_bracket(c)){
                t.type = 'e';
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
            else if(is_a_quote(c)){
                t.type = 's';
                counter++;
                char s = file.body[counter];
                while(!is_a_quote(s)){
                    t.data.body[t.data.length] = file.body[counter];
                    t.data.length++;
                    counter++;
                    s = file.body[counter];
                }
            }
            else if(is_a_function(c)){
                t.type = c;
            }
            else if(is_a_letter(c)){
                t.type = 'k';
                while(is_a_letter(c)){
                    t.data.body[t.data.length] = file.body[counter];
                    t.data.length++;
                    counter++;
                    c = file.body[counter];
                }
                counter--;
            }
            content->tokens[content->length] = t;
            content->length++;
        }
        counter++;
    }

    return content;
}
