#ifndef _STRUCTURES_H
#define _STRUCTURES_H

struct String {
    char body[80];
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
    long ln;
    struct String str;
};

struct Value {
    char type;
    union Data data;
};

struct Keyval {
    struct Value key;
    struct Value val;
};

struct Map {
    int size;
    struct Keyval * members[];
};

struct Tree {
    char type;
    int size;
    struct Value content;
    struct Tree * children[10];
    struct Tree * parent;
};

#endif /* _STRUCTURES_H */
