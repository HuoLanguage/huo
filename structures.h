#ifndef _STRUCTURES_H
#define _STRUCTURES_H

struct String {
    char body[50];
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

struct Tree {
    char type;
    int size;
    struct String content;
    struct Tree * children[10];
    struct Tree * parent;
};

struct Array {
    int length;
    struct Tree trees[1000];
};

#endif /* _STRUCTURES_H */
