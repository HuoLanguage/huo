#include <assert.h>
#include "../structures/structures.h"
#include "../execute.h"
#include "../base_util.h"
#include "../config.h"

bool read_file_to(struct String *file_contents, const char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL){
        return false;
    }
    char c;
    while ((c = fgetc(fp)) != EOF){
        if (c == 0) {
            ERROR("Null byte in input file");
        } else {
            RESIZE(file_contents->body, file_contents->length + 1);
            file_contents->body[file_contents->length] = c;
            file_contents->length++;
        }
    }
    fclose(fp);

    // Null character at end
    RESIZE(file_contents->body, file_contents->length + 1);
    file_contents->body[file_contents->length] = 0;

    assert(string_is_sane(file_contents));
    return true;
}

struct String read_file(struct String file_name){
    struct String file = string_from_chars(NULL);
    char *s = string_to_chars(&file_name);
    if (!read_file_to(&file, s)) {
        ERROR("Cannot find file: \"%s\"", s);
    }
    return file;
}
