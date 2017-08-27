#include <string.h>
#include <errno.h>
#include <sys/param.h>

#if defined(_POSIX_VERSION) || defined(__linux__) || defined(__APPLE__)
#include <libgen.h>
#include "base_util.h"

char *get_exe_path(const char *called_name) {
    char *path_to_exe = realpath(called_name, NULL);

    if (path_to_exe == NULL) {
        ERROR("Error getting real path: %d (%s)", errno, strerror(errno));
    }

    return path_to_exe;
}

char *get_path_dir(char *path) {
    char *temp = dirname(path);
    if (temp == NULL) {
        ERROR("Error splitting directory: %s", path);
    }
    char *dup = o_strdup(temp); // dirname strings should not be freed
    if (dup == NULL) {
        ERROR("Malloc failure");
    }
    return dup;
}

char *path_merge(const char *dir, const char *rest) {
    // Bleh
    char *path = ARR_MALLOC(strlen(dir) + 1 + strlen(rest) + 1, char); // sizeof(char) is defined as 1, I know.
    if (path == NULL) {
        ERROR("Malloc failure");
    }
    path[0] = 0;
    strcat(path, dir);
    strcat(path, "/");
    strcat(path, rest);
    return path;
}
#elif  defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
// UNTESTED!!
#include <Windows.h>
#include "base_util.h"


char *get_exe_path(const char *called_name) {
    char *buffer = malloc_or_die(MAX_PATH);
    GetModuleFileName(NULL, buffer, MAX_PATH) ;

    return buffer;
}

char *get_path_dir(char *path) {
    // Bleh
    char *temp = o_strdup(path);
    size_t len = strlen(temp);
    do {
        temp[len] = 0;
        if (len <= 1) {
            ERROR("Could not find directory of %s", path);
        }
    } while (temp[--len] != '/' && temp[--len] != '\\');
    temp[len] = 0;
    return temp;
}

char *path_merge(const char *dir, const char *rest) {
    char *path = ARR_MALLOC(strlen(dir) + 1 + strlen(rest) + 1, char);
    if (path == NULL) {
        ERROR("Malloc failure");
    }
    path[0] = 0;
    strcat(path, dir);
    strcat(path, "\\");
    strcat(path, rest);
    return path;
}
#else
#error "Unknown system!"
#endif
