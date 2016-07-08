#ifndef _HUO_STRING_H
#define _HUO_STRING_H
#include <stdbool.h>
// _STRING_H already exists!

struct String {
    size_t length;
    char *body;
};

/* C compile-time constant madness*/
#define STRLEN(s) ( (sizeof(s)/sizeof(s[0])) - sizeof(s[0]) )
#define STR_NEW(str)\
{\
        .length = STRLEN(str),\
        .body=(str)\
}

struct String string_from_char(char c);
struct String string_from_chars(const char *str);
bool string_is_sane(const struct String *const s);
struct String string_copy_stack(struct String *from);
struct String *string_copy_heap(struct String *from);
void string_copy_to(struct String *to, struct String *from);
struct String string_concat_stack(struct String *a, struct String *b);
struct String *string_concat_heap(struct String *a, struct String *b);
void string_concat_to(struct String *to, struct String *from);
bool string_matches_heap(struct String *base, struct String *compare);
bool string_matches_stack(struct String base, struct String compare);
char *string_to_chars(const struct String *s);
size_t string_length(struct String *s);
char string_index(struct String *s, size_t i);
bool string_contains(char ch, struct String* string);

unsigned long string_hash_code(struct String *s);
unsigned long string_hash_code_vv(void *s);

bool string_matches_vv(void *base, void *compare);

void string_free(struct String *const string);

#endif
