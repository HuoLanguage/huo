#include <string.h>
#include <stddef.h>
#include <assert.h>
#include "constants.h"
#include "config.h"

struct Tokens * tokenize(struct String file, struct Tokens *content){
    assert(string_is_sane(&file));
    size_t counter = 0;
    while (counter < file.length){
        char c = file.body[counter];
        if(c != ' ' && c != '\n'){
            struct Token t = {
                .data = {
                    .length = 0,
                    .body = NULL
                }
            };
            assert(string_is_sane(&t.data));
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
                while(is_a_number(c) || c == dot_const){
                    if (counter >= file.length) {
                        counter += 1; // to counter below "counter--"
                        break;
                    }
                    RESIZE(t.data.body, t.data.length+1);
                    t.data.body[t.data.length] = file.body[counter];
                    t.data.length++;
                    counter++;
                    c = file.body[counter];
                }
                RESIZE(t.data.body, t.data.length+1);
                t.data.body[t.data.length] = 0;
                assert(string_is_sane(&t.data));
                counter--;
            }
            else if(is_a_quote(c)){
                t.type = 's';
                counter++;
                if (counter >= file.length) {
                    ERROR("Mismatched quotes");
                }
                char s = file.body[counter];
                while(!is_a_quote(s)){
                    if (counter >= file.length) {
                        ERROR("Mismatched quotes");
                    }
                    RESIZE(t.data.body, t.data.length+1);
                    t.data.body[t.data.length] = file.body[counter];
                    t.data.length++;
                    counter++;
                    s = file.body[counter];
                }
                if (t.data.length != 0) {
                    RESIZE(t.data.body, t.data.length+1);
                    t.data.body[t.data.length] = 0;
                }
                assert(string_is_sane(&t.data));
            }
            else if(is_a_function(c)){
                t.type = c;
            }
            else if(is_a_letter(c)){
                t.type = 'k';
                while(is_a_letter(c)){
                    if (counter >= file.length) {
                        counter += 1; // to counter below "counter--"
                        break;
                    }
                    RESIZE(t.data.body, t.data.length+1);
                    t.data.body[t.data.length] = file.body[counter];
                    t.data.length++;
                    counter++;
                    c = file.body[counter];
                }
                RESIZE(t.data.body, t.data.length+1);
                t.data.body[t.data.length] = 0;
                assert(string_is_sane(&t.data));
                counter--;
            }
            RESIZE(content->tokens, content->length + 1);
            content->tokens[content->length] = t;
            content->length++;
        }
        counter++;
    }

    return content;
}
