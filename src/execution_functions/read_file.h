#ifndef _READ_FILE_H
#define _READ_FILE_H

#include "../structures/structures.h"

bool read_file_to(struct String *file_contents, const char *filename);
struct String read_file(struct String);

#endif
