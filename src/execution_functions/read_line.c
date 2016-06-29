#include <assert.h>
#include "../structures/structures.h"
#include "../structures/string.h"
#include "../structures/value.h"
#include "../constants.h"
#include "../execute.h"
#include "../core_functions.h"

struct Value read_line(struct Value * string){
    // read line takes a string to display as prompt
    // but displays it without quotes
    string->type = KEYWORD;
    print(*string);
    string->type = STRING;

    struct String input = {
        .length = 0,
        .body = NULL
    };

    struct Value result = {
        .type = STRING
    };

    char c;
    while ((c = getchar()) != '\n'){
        if (c == 0) {
            ERROR("Null byte in input file");
        } else if(c != '\n'){
            RESIZE(input.body, input.length + 1);
            input.body[input.length] = c;
            input.length++;
        }
    }
    RESIZE(input.body, input.length + 1);
    input.body[input.length] = 0;

    assert(string_is_sane(&input));

    result.data.str = input;
    return result;
}
