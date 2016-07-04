#ifndef _HUO_H
#define _HUO_H

char *get_exe_path(const char *called_name);
char *get_path_dir(char *path);
char *path_merge(const char *dir, const char *rest);
int main(int argc, char const *argv[]);

#endif
