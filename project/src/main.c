#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "simple.h"
#define ERR_ARGS_COUNT (-1)
#define ERR_WRONG_FLG (-2)

#define TST_FOO_FIX     1
#define TST_FOO_IMPL    2
#define TST_MOD_IMPL    3


int main(int argc, const char** argv) {
    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

    int test_case = atoi(argv[1]);
    const char* data = argv[2];

    switch (test_case) {
        case TST_FOO_FIX: {
            size_t res = custom_strlen(data);
            printf("%zu\n", res);
            break;
        }
        case TST_FOO_IMPL: {
            if (argc != 4) {
                return ERR_ARGS_COUNT;
            }
            int base = atoi(data);
            int pow =  atoi(argv[3]);
            int res = custom_pow(base, pow);
            printf("%i\n", res);
            break;
        }
        case TST_MOD_IMPL: {
            if (argc != 3) {
                return ERR_ARGS_COUNT;
            }
            int num = atoi(data);
            int res = simple_number_check(num);
            printf("%i\n", res);
            break;
        }
        default: {
            return ERR_WRONG_FLG;
        }
    }

    return 0;
}
