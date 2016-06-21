#ifndef _STRUCTURES_H
#define _STRUCTURES_H

struct String {
    char body[256];
    int length;
};

struct Token {
    char type;
    struct String data;
};

struct Tokens {
    struct Token tokens[10000];
    int length;
    int counter;
};

union Data {
    char bl;
    long ln;
    float fl;
    struct String str;
    struct Value_array * array;
};

struct Value {
    char type; // [f]loat [b]ool [l]ong [s]tring [a]rray [u]ndefined
    union Data data;
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
};

#endif /* _STRUCTURES_H */
