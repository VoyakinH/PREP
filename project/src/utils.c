#include <string.h>
#include "utils.h"

size_t custom_strlen(const char* str) {
    return strlen(str);
}

int custom_pow(int base, int power) {
    int minus = 0;
    if (power == 0) {
        return 1;
    } else if (power == 1) {
        return base;
    } else if (power < 0) {
        minus++;
        power *= -1;
    }
    float res = base;
    while (power > 1) {
        res *= base;
        power--;
    }
    if (minus == 1)
        res = 1 / res;
    return (int) res;
}
