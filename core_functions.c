#include <stdlib.h>
#include "structures.h"

struct Value add(struct Value a, struct Value b){
    a.data.ln = a.data.ln + b.data.ln;
    return a;
}

struct Value mul(struct Value a, struct Value b){
    a.data.ln = a.data.ln * b.data.ln;
    return a;
}

struct Value sub(struct Value a, struct Value b){
    a.data.ln = a.data.ln - b.data.ln;
    return a;
}

struct Value divide(struct Value a, struct Value b){
    a.data.ln = (long)(a.data.ln/b.data.ln);
    return a;
}

void print(struct Value a){
    if(a.type == 's'){
        printf("%s\n", a.data.str.body);
    } else {
        printf("%ld\n", a.data.ln);
    }

}
