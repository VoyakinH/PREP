#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(void) {
    Matrix *matrix = create_matrix_from_file("base.txt");
    if (!matrix) {
        return 1;
    }
    double val = 0;
    get_elem(matrix, 0, 0, &val);
    Matrix *res = inv(matrix);
    Matrix *res_mul = mul(matrix, res);
    if (!res || !res_mul) {
        free_matrix(matrix);
        free_matrix(res);
        free_matrix(res_mul);
        return 2;
    }
    if (set_elem(matrix, 0, 0, -1) != 0) {
        free_matrix(matrix);
        free_matrix(res);
        free_matrix(res_mul);
        return 3;
    }
    res = sub(matrix, res_mul);
    res_mul = sum(matrix, res_mul);
    if (!res || !res_mul) {
        free_matrix(matrix);
        free_matrix(res);
        free_matrix(res_mul);
        return 4;
    }
    free_matrix(matrix);
    free_matrix(res);
    free_matrix(res_mul);
    return 0;
}

