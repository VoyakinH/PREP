#include <string.h>
#include "utils.h"

size_t custom_strlen(const char* str) {
    return strlen(str);
}

int custom_pow(int base, int power) {
    if (power == 0) {
        return 1;
    }
    if (power < -1) {
        return 0;
    }
    int res = base;
    while (power > 1) {
        res *= base;
        power--;
    }
    return res;
}
