#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#include "config.h"
#include "base_util.h"

#undef malloc
void *_check_malloc(size_t s) {
    static unsigned int i = 0;
    static char c = 'n';
    if (i == 0) {
        if (scanf("%c%ui\n", &c, &i) <= 0)
            ERROR("?");
        if (i == 0)
            ERROR("?");
    }
    i -= 1;
    if (c == 'n') {
        return malloc(s);
    } else if (c == '+') {
        if (s > SIZE_MAX / 2 - 1)
            s = SIZE_MAX;
        else {
            s = s * 2 + 1;
        }
        return malloc(s);
    } else {
        return NULL;
    }
}