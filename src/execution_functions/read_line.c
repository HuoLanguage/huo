#include <assert.h>
#include "../structures/structures.h"
#include "../structures/string.h"
#include "../structures/value.h"
#include "../constants.h"
#include "../execute.h"
#include "../core_functions.h"
#include "../config.h"

struct String read_line(struct Value * string){
    // read line takes a string to display as prompt
    // but displays it without quotes
    string->type = KEYWORD;
    print(*string);
    string->type = STRING;

    struct String input = {
        .length = 0,
        .body = NULL
    };

    char c;
    while ((c = getchar()) != '\n' && c != EOF){
        if (c == 0) {
            ERROR("Null byte in input file");
        } else if(c != '\n'){
            RESIZE(input.body, input.length + 1);
            input.body[input.length] = c;
            input.length++;
        }
    }
    if (input.length == 0) {
        ERROR("Input closed");
    }
    RESIZE(input.body, input.length + 1);
    input.body[input.length] = 0;

    assert(string_is_sane(&input));

    return input;
}
