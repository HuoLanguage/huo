#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/param.h>
#include "constants.h"
#include "tokenizer.h"
#include "parser.h"
#include "structures/structures.h"
#include "execution_functions/read_file.h"
#include "execute.h"
#include "store_defs.h"
#include "base_util.h"
#include "huo.h"
#include "config.h"

#if defined(_POSIX_VERSION) || defined(__linux__) || defined(__APPLE__)

#include <libgen.h>
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
    char *dup = strdup(temp); // dirname strings should not be freed
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
#elif  defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
// UNTESTED!!
#include <Windows.h>

char *get_exe_path(const char *called_name) {
    char *buffer = malloc_or_die(MAX_PATH);
    GetModuleFileName(NULL, buffer, MAX_PATH) ;

    return buffer;
}

char *get_path_dir(char *path) {
    // Bleh
    char *temp = strdup(path);
    size_t len = strlen(temp);
    do {
        temp[len] = 0;
        if (len <= 1) {
            ERROR("Could not find directory of %s", path);
        }
    } while (temp[--len] != '/');
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

int main(int argc, char const *argv[]) {
    bool help_flag = false;
    bool command_flag = false;
    bool error_flag = false;
    bool file_flag = false;

    struct String to_execute = {
        .length = 0,
        .body = NULL
    };

    // This is a little "brittle", but it works for now
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            help_flag = true;
        } else if (!strcmp(argv[i], "-c")) {
            if (i == argc - 1) {
                fprintf(stderr, "Argument expected for %s option\n", argv[i]);
                error_flag = true;
            } else if (command_flag == true) {
                fprintf(stderr, "Multiple commands specified\n");
                error_flag = true;
            } else {
                command_flag = true;
                to_execute = string_from_chars(argv[++i]);
            }
        } else if (i == argc - 1 && !file_flag) {
            file_flag = true;
            if (!read_file_to(&to_execute, argv[i])) {
                ERROR("Error opening file: %d (%s)", errno, strerror(errno));
            }
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            help_flag = true;
            error_flag = true;
        }
        if (error_flag) {
            break;
        }
    }
    if (command_flag && file_flag) {
        fprintf(stderr, "Command passed in but file also specified\n");
        error_flag = true;
    }

    if (!command_flag && !file_flag && !help_flag) {
        assert (argc >= 1);

        char *path_to_exe = get_exe_path(argv[0]);

        char *path_to_exe_dir = get_path_dir(path_to_exe);

        char *path_to_repl = path_merge(path_to_exe_dir, "repl.huo");

        // printf("%s \n", path_to_exe_dir);

        file_flag = true;

        if (!read_file_to(&to_execute, path_to_repl)) {
            ERROR("Error opening repl.huo: %d (%s)", errno, strerror(errno));
        }

        free(path_to_repl);
        free(path_to_exe_dir);
        free(path_to_exe);
    }

    if (help_flag || error_flag) {
        fprintf(stderr,
"usage: huo [FILE | -c cmd | -h | --help]\n"
"Arguments:\n"
"-c cmd     program passed in as string\n"
"FILE       program read in as file\n"
"-h, --help print this help message and exit\n");
        return error_flag ? 1 : 0; // Redundant for now, but reminder to maybe return a different error code later
    }

    struct Tokens t = {
        .tokens = NULL,
        .length = 0,
        .counter = 0
    };

    struct Tokens * tokens = tokenize(to_execute, &t);
    //for(size_t i = 0; i < tokens->length; i++){
    //     printf("%c", tokens->tokens[i].type);
    //}

    struct Tree root;
    root.type = 'r';
    root.size = 0;
    root.children = NULL;

    parse(&root, tokens, true);
    // this prints the AST for reference
    // printTree(&root);
    // printf("\n");
    hash_table *defined = hash_table_new(&string_hash_code_vv, &string_matches_vv);
    struct Scopes * scopes = malloc_or_die(sizeof(struct Scopes));
    scopes->scopes = NULL;
    RESIZE(scopes->scopes, 1);
    scopes->size = 1;
    scopes->current = 0;

    scopes->scopes[0] = hash_table_new(value_keyword_hash_code, value_keyword_equality);

    size_t num_defs = store_defs(&root, defined);
    for(size_t i = num_defs; i < root.size; i++){
        execute(root.children[i], defined, scopes, RECURSE_MAX);
    }
    return 0;
}
