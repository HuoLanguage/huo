#ifndef _HUO_STRING_H
#define _HUO_STRING_H
#include <stdbool.h>
// _STRING_H already exists!

struct String {
    int length;
    char *body;
};

/* C compile-time constant madness*/
#define STRLEN(s) ( (sizeof(s)/sizeof(s[0])) - sizeof(s[0]) )
#define STR_NEW(str)\
{\
        .length = STRLEN(str),\
        .body=(str)\
}
/*
struct String string_new(char *str);
*/
bool string_is_sane(struct String *s);
struct String string_copy_stack(struct String *from);
struct String *copy_string_heap(struct String *from);
void string_copy_to(struct String *to, struct String *from);
void string_concat_to(struct String *to, struct String *from);
bool string_matches(struct String *base, struct String *compare);

#endif