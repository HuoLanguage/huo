#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include "string.h"
#include "value.h"

struct Token {
    char type;
    struct String data;
};

struct Tokens {
    struct Token tokens[10000];
    int length;
    int counter;
};

struct Value_array {
    int size;
    struct Value * values[1000];
};

struct Keyval {
    struct Value * key;
    struct Value * val;
};

struct Tree_map {
    struct String * names [250];
    struct Tree * trees [250];
    int size;
};

struct Map {
    int size;
    struct Keyval * members[200];
};

struct Tree {
    char type; // [o]pen, [f]unction [k]eyword [c]lose [s]tring [n]umber [b]racket [e]nd bracket
    int size;
    struct Value content;
    struct Tree * children[200];
    struct Tree * parent;
};

struct Execution_bundle {
    struct Tree * ast;
    struct Tree_map * defined;
    struct Map * let_map;
    int max_depth;
};

#endif /* _STRUCTURES_H */
