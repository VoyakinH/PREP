#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_
#define BASE_STR_LEN 50

typedef struct EMAIL {
    char *from;
    char *to;
    char *date;
    int parts;
} EMAIL;

int init_struct(EMAIL **base, int len_for_from, int len_for_to, int len_for_date);
void free_struct(EMAIL *base);
void output_struct(EMAIL *base);
int is_title(FILE *f, char **str);
int parse(FILE *f, char **parsed);
int get_title_text(FILE *f, char **parsed, char **got_str);
void copy_to_struct(EMAIL **base, char *from, char *to, char *date, int parts);
int read_from_file(FILE *f, int *parts, char **from, char **to, char **date, char **boundary);
int initialize_arrays(char **from, char **to, char **date, char **boundary);

#endif  // PROJECT_INCLUDE_UTILS_H_
