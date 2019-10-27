#define _GNU_SOURCE char *strcasestr(const char *haystack, const char *needle)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utils.h>

int init_struct(EMAIL **base, int len_for_from, int len_for_to, int len_for_date) {
    *base = malloc(sizeof(EMAIL));
    if (*base == NULL) {
        return 1;
    }

    (*base)->from = malloc(sizeof(char) * (len_for_from + 1));
    if (!(*base)->from) {
        free(base);
        return 2;
    }

    (*base)->to = malloc(sizeof(char) * (len_for_to + 1));
    if (!(*base)->to) {
        free((*base)->from);
        free(base);
        return 3;
    }

    (*base)->date = malloc(sizeof(char) * (len_for_date + 1));
    if (!(*base)->date) {
        free((*base)->from);
        free((*base)->to);
        free((*base));
        return 4;
    }

    return 0;
}

void copy_to_struct(EMAIL **base, char *from, char *to, char *date, int parts) {
    strncpy((*base)->from, from, strlen(from) + 1);
    strncpy((*base)->to, to, strlen(to) + 1);
    strncpy((*base)->date, date, strlen(date) + 1);
    (*base)->parts = parts;

    return;
}

void free_struct(EMAIL *base) {
    free(base->from);
    free(base->to);
    free(base->date);
    free(base);

    return;
}

void output_struct(EMAIL *base) {
    fprintf(stdout, "%s|%s|%s|%d\n", base->from, base->to, base->date, base->parts);

    return;
}

int is_title(FILE *f, char **str) {
    if (!str || strlen(*str) == 0) {
        return -1;
    }

    if (!strcasestr(*str, "from:") && !strcasestr(*str, "to:")
        && !strcasestr(*str, "date:") && (*str)[strlen(*str) - 1] != ':') {
        return -1;
    }

    if (strcasestr(*str, "from:") - *str == 0) {
        fseek(f, -strlen(*str) + 5, SEEK_CUR);
        return 0;
    }

    if (strcasestr(*str, "to:") - *str == 0) {
        fseek(f, -strlen(*str) + 3, SEEK_CUR);
        return 1;
    }

    if (strcasestr(*str, "date:") - *str == 0) {
        fseek(f, -strlen(*str) + 5, SEEK_CUR);
        return 2;
    }

    if (strcasecmp(*str, "Content-Type:") == 0) {
        return 3;
    }

    return 4;
}

int parse(FILE *f, char **parsed) {
    *parsed = calloc(BASE_STR_LEN + 1, sizeof(char));
    if (!*parsed) {
        return 1;
    }

    char buff[BASE_STR_LEN + 1];

    for (int i = 0; i < BASE_STR_LEN; i++) {
        buff[i] = '1';
    }
    buff[50] = '\0';

    while (strlen(buff) == BASE_STR_LEN) {
        if (fscanf(f, "%50s", buff) != 1) {
            break;
        }

        strncat(*parsed, buff, strlen(buff) + 1);
        if (strlen(buff) != BASE_STR_LEN || fgetc(f) == ' ') {
            break;
        }

        fseek(f, ftell(f) - 1, SEEK_SET);

        char elem = fgetc(f);
        if (elem == ' ' || elem == '\n' || elem == '\r') {
            break;
        }

        fseek(f, ftell(f) - 1, SEEK_SET);

        *parsed = realloc(*parsed, sizeof(char) * (strlen(*parsed) + BASE_STR_LEN + 1));
        if (!*parsed) {
            return 2;
        }
    }

    return 0;
}

int get_title_text(FILE *f, char **parsed, char **got_str) {
    char *buff = NULL;

    if (parse(f, &buff) != 0) {
        return 0;
    }
    strncpy(*parsed, "\0", 1);

    while (is_title(f, &buff) == -1) {
        char elem = fgetc(f);
        int count = 0;
        while (elem == ' ' || elem == '\n' || elem == '\r') {
            if (elem == ' ') {
                count++;
            }
            elem = fgetc(f);
        }
        *parsed = realloc(*parsed, (strlen(*parsed) + strlen(buff) + 3 + count) * sizeof(char));
        if (!parsed) {
            return 1;
        }
        strncat(*parsed, buff, strlen(buff) + 1);
        fseek(f, -3, SEEK_CUR);
        if (fgetc(f) == '\n') {
            if (fgetc(f) == '\n') {
                parse(f, &buff);
            break;
            }
            fseek(f, -1, SEEK_CUR);
        }
        fseek(f, 1, SEEK_CUR);
        for (int i = 0; i < count; i++) {
            strncat(*parsed, " ", 1);
        }
        if (feof(f)) {
            break;
        }
        if (parse(f, &buff) != 0) {
            break;
        }
    }
    *got_str = calloc(strlen(*parsed) + 1, sizeof(char));
    strncpy(*got_str, *parsed, strlen(*parsed) + 1);
    strncpy(*parsed, buff, strlen(buff) + 1);
    return 0;
}

static int is_from(FILE *f, char **parsed, char elem[], char **from, int *stop_read) {
    if (strlen(*from) == 0) {
        if (is_title(f, parsed) == 0 && strlen(elem) != 4) {
            if (get_title_text(f, parsed, from) != 0) {
                return 1;
            }
            *stop_read = 1;
        }
    }

    return 0;
}

static int is_to(FILE *f, char **parsed, char elem[], char **to, int *stop_read) {
    if (strlen(*to) == 0) {
        if (is_title(f, parsed) == 1 && strlen(elem) != 4) {
            if (get_title_text(f, parsed, to) != 0) {
                return 1;
            }
            *stop_read = 1;
        }
    }

    return 0;
}

static int  is_date(FILE *f, char **parsed, char elem[], char **date, int *stop_read) {
    if (strlen(*date) == 0) {
        if (is_title(f, parsed) == 2 && strlen(elem) != 4) {
            if (get_title_text(f, parsed, date) != 0) {
                return 1;
            }
            *stop_read = 1;
        }
    }

    return 0;
}

static int  is_boundary(FILE *f, char **parsed, char elem[], char **boundary, int *stop_read) {
    if (strlen(*boundary) == 0) {
        if (is_title(f, parsed) == 3 && strlen(elem) != 4) {
            if (get_title_text(f, parsed, boundary) != 0) {
                return 1;
            }
            if (!strcasestr(*boundary, "multipart") || strcasestr(*boundary, "xboundary") != NULL) {
                strncpy(*boundary, "\0", 1);
            } else {
                *boundary = strcasestr(*boundary, "boundary") + 8;

                if (strstr(*boundary, "=") != NULL) {
                    *(strstr(*boundary, "=")) = ' ';
                }

                char *save_ptr = NULL;
                *boundary = strtok_r(*boundary, "\"; ", &save_ptr);
                (*boundary)[strlen(*boundary)] = '\0';
            }
            *stop_read = 1;
        }
    }

    return 0;
}

int initialize_arrays(char **from, char **to, char **date, char **boundary) {
    *from = calloc(1, sizeof(char));
    if (!*from) {
        return 1;
    }

    *to = calloc(1, sizeof(char));
    if (!*to) {
        return 2;
    }

    *date = calloc(1, sizeof(char));
    if (!*date) {
        return 3;
    }

    *boundary = calloc(1, sizeof(char));
    if (!*boundary) {
        return 4;
    }

    strncpy(*from, "\0", 1);
    strncpy(*to, "\0", 1);
    strncpy(*date, "\0", 1);
    strncpy(*boundary, "\0", 1);
    return 0;
}

static int read_elem_and_parts_logic(FILE *f, int stop_read, char elem[],
                                     char **parsed, char **boundary,
                                     int *parts, int *last_boundary) {
    if (stop_read == 0) {
        if (ftell(f) != 0) {
            fseek(f, -1, SEEK_CUR);
            fgets(elem, 5, f);
            fseek(f, -strlen(elem) + 1, SEEK_CUR);
        } else {
            elem[0] = '\0';
        }

        if (strstr(*parsed, *boundary) != NULL) {
            *last_boundary = 1;
        }
        int buff = ftell(f);

        if (parse(f, parsed) != 0) {
            return 1;
        }

        if (strlen(*parsed) == 0 && *last_boundary == 0 && (ftell(f) - buff == 1 || *parts > 0)) {
            *parts = *parts + 1;
            if (*parts == 0) {
                *parts = 1;
            }
        } else {
            *last_boundary = 0;
        }
    }
    return 0;
}

int read_from_file(FILE *f, int *parts, char **from, char **to, char **date, char **boundary) {
    char elem[5];
    char *parsed = NULL;

    int stop_read = 0;
    int last_boundary = 0;

    while (!feof(f)) {
        if (read_elem_and_parts_logic(f, stop_read, elem, &parsed, boundary, parts, &last_boundary) != 0) {
            free(parsed);
            return 1;
        }

        stop_read = 0;

        if (is_from(f, &parsed, elem, from, &stop_read) != 0) {
            free(parsed);
            return 2;
        }

        if (is_to(f, &parsed, elem, to, &stop_read) != 0) {
            free(parsed);
            return 3;
        }

        if (is_date(f, &parsed, elem, date, &stop_read) != 0) {
            free(parsed);
            return 4;
        }

        if (is_boundary(f, &parsed, elem, boundary, &stop_read) != 0) {
            free(parsed);
            return 5;
        }

        if (strlen(*boundary) != 0) {
            if (strstr(parsed, *boundary)) {
                *parts = *parts + 1;
                if (strlen(parsed) > strlen(*boundary) + 2)
                    break;
            }
        }
        if (*parts != -1) {
            stop_read = 0;
        }
    }

    if (*parts == -1 && strlen(*boundary) == 0) {
        *parts = 1;
    } else if (*parts == -1) {
        *parts = 0;
    }

    free(parsed);
    return 0;
}
