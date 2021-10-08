#ifndef _PATH_UTILS_H
#define _PATH_UTILS_H

char *get_exe_path(const char *called_name);
char *get_path_dir(char *path);
char *path_merge(const char *dir, const char *rest);

#endif
