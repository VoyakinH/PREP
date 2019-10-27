#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];
    FILE *f = fopen(path_to_eml, "r");
    if (!f) {
        return 1;
    }

    char *from = NULL;
    char *to = NULL;
    char *date = NULL;
    char *boundary = NULL;

    if (initialize_arrays(&from, &to, &date, &boundary) != 0) {
        free(from);
        free(to);
        free(date);
        fclose(f);
        return 1;
    }

    int parts = -1;

    if (read_from_file(f, &parts, &from, &to, &date, &boundary) != 0) {
        free(from);
        free(to);
        free(date);
        fclose(f);
        return 2;
    }

    EMAIL *base;

    if (init_struct(&base, strlen(from), strlen(to), strlen(date)) != 0) {
        free_struct(base);
        return 3;
    }

    copy_to_struct(&base, from, to, date, parts);
    output_struct(base);

    free_struct(base);
    fclose(f);

    free(from);
    free(to);
    free(date);
    return 0;
}
