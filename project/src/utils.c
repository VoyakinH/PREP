#include <string.h>
#include "utils.h"

size_t custom_strlen(const char* str) {
    return strlen(str);
}

int custom_pow(int base, int power) {
    if (power == 0) {
        return 1;
    }
    
    float res = base;
    float base_f = base;
    
    if (power < 0) {
        res = 1 / base;
        power *= -1;
        base_f = 1 / base_f;
    }
    
    while (power > 1) {
        res *= base_f;
        power--;
    }
    return (int) res;
}
