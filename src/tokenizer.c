#include <string.h>
#include <stddef.h>
#include <assert.h>
#include "constants.h"
#include "config.h"
#include "./structures/token.h"

struct Tokens * tokenize(struct String file, struct Tokens *content){
    assert(string_is_sane(&file));
    struct Token null_token = {
        .data = {
            .length = 0,
            .body = NULL
        }
    };
    struct Token cur_token = null_token;

    for (size_t counter = 0; counter < file.length; counter++) {
        char c = file.body[counter];

        if (cur_token.data.length == 0 || !can_be_extended(cur_token.data, c, cur_token.type)) {
            if (cur_token.data.length != 0) {
                RESIZE(cur_token.data.body, cur_token.data.length + 1);
                cur_token.data.body[cur_token.data.length] = 0;

                RESIZE(content->tokens, content->length + 1);
                content->tokens[content->length] = cur_token;
                content->length++;
            }
            cur_token = null_token;
            cur_token.type = get_token_type(c);
        }
        RESIZE(cur_token.data.body, cur_token.data.length + 1);
        cur_token.data.body[cur_token.data.length] = c;
        cur_token.data.length += 1;

    }
    if (cur_token.data.length != 0) {
        RESIZE(cur_token.data.body, cur_token.data.length + 1);
        cur_token.data.body[cur_token.data.length] = 0;

        RESIZE(content->tokens, content->length + 1);
        content->tokens[content->length] = cur_token;
        content->length++;
    }

    cur_token = null_token;
    cur_token.type = T_EOF;

    RESIZE(content->tokens, content->length + 1);
    content->tokens[content->length] = cur_token;
    content->length++;

    return content;
}
