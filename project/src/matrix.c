#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

Matrix *create_matrix(size_t rows, size_t cols) {
    Matrix *matrix = malloc(sizeof(Matrix));

    double *a_new = calloc(rows * cols, sizeof(double));

    if (!a_new) {
        free_matrix(matrix);
        return NULL;
    }
    matrix->a = a_new;

    matrix->rows = rows;
    matrix->cols = cols;
    return matrix;
}

Matrix *create_matrix_from_file(const char *path_file) {
    Matrix *matrix = malloc(sizeof(Matrix));

    FILE *f = fopen(path_file, "r");
    if (!f) {
        printf("Can not open file: %s\n", path_file);
        free_matrix(matrix);
        return NULL;
    }

    fscanf(f, "%zu %zu", &matrix->rows, &matrix->cols);

    double *a_new = calloc(matrix->rows * matrix->cols, sizeof(double));
    if (!a_new) {
        fclose(f);
        free_matrix(matrix);
        return NULL;
    }
    matrix->a = a_new;
    double val = 0;

    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            fscanf(f, "%lf", &val);
            set_elem(matrix, i, j, val);
        }
    }

    fclose(f);
    return matrix;
}

int free_matrix(Matrix *matrix) {
    free(matrix->a);
    free(matrix);

    return 0;
}

int get_rows(const Matrix *matrix, size_t *rows) {
    if (!matrix) {
        return 1;
    }

    *rows = matrix->rows;
    return 0;
}

int get_cols(const Matrix *matrix, size_t *cols) {
    if (!matrix) {
        return 1;
    }

    *cols = matrix->cols;
    return 0;
}

int get_elem(const Matrix *matrix, size_t row, size_t col, double *val) {
    if (!matrix || row >= matrix->rows || col >= matrix->cols) {
        return 1;
    }

    *val = matrix->a[row * matrix->cols + col];
    return 0;
}

int set_elem(Matrix *matrix, size_t row, size_t col, double val) {
    if (!matrix || row >= matrix->rows || col >= matrix->cols) {
        return 1;
    }

    matrix->a[row * matrix->cols + col] = val;
    return 0;
}

Matrix *mul_scalar(const Matrix *matrix, double val) {
    size_t rows = 0;
    size_t cols = 0;

    int rc = get_rows(matrix, &rows);
    rc += get_cols(matrix, &cols);
    if (rc != 0 || rows == 0 || cols == 0) {
        return NULL;
    }

    double elem = 0;

    Matrix *new_matrix = create_matrix(rows, cols);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            get_elem(matrix, i, j, &elem);
            set_elem(new_matrix, i, j, elem * val);
        }
    }
    return new_matrix;
}

Matrix *transp(const Matrix *matrix) {
    size_t rows = 0;
    size_t cols = 0;

    int rc = get_rows(matrix, &rows);
    rc += get_cols(matrix, &cols);
    if (rc != 0 || rows == 0 || cols == 0) {
        return NULL;
    }

    double elem = 0;

    Matrix *new_matrix = create_matrix(cols, rows);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            get_elem(matrix, i, j, &elem);
            set_elem(new_matrix, j, i, elem);
        }
    }

    return new_matrix;
}

Matrix *sum(const Matrix *l, const Matrix *r) {
    size_t rows_l = 0;
    size_t cols_l = 0;
    size_t rows_r = 0;
    size_t cols_r = 0;

    int rc = get_rows(l, &rows_l);
    rc += get_cols(l, &cols_l);
    rc += get_rows(r, &rows_r);
    rc += get_cols(r, &cols_r);
    if (rows_l != rows_r || cols_l != cols_r || !l || !r || rc != 0) {
        return NULL;
    }

    Matrix *new_matrix = create_matrix(rows_l, cols_l);

    double elem_l = 0;
    double elem_r = 0;

    for (size_t i = 0; i < rows_l; i++) {
        for (size_t j = 0; j < cols_l; j++) {
            get_elem(l, i, j, &elem_l);
            get_elem(r, i, j, &elem_r);
            set_elem(new_matrix, i, j, elem_l + elem_r);
        }
    }

    return new_matrix;
}

Matrix *sub(const Matrix *l, const Matrix *r) {
    size_t rows_l = 0;
    size_t cols_l = 0;
    size_t rows_r = 0;
    size_t cols_r = 0;

    int rc = get_rows(l, &rows_l);
    rc += get_cols(l, &cols_l);
    rc += get_rows(r, &rows_r);
    rc += get_cols(r, &cols_r);
    if (rows_l != rows_r || cols_l != cols_r || !l || !r || rc != 0) {
        return NULL;
    }

    Matrix *new_matrix = create_matrix(rows_l, cols_l);

    double elem_l = 0;
    double elem_r = 0;

    for (size_t i = 0; i < rows_l; i++) {
        for (size_t j = 0; j < cols_l; j++) {
            get_elem(l, i, j, &elem_l);
            get_elem(r, i, j, &elem_r);
            set_elem(new_matrix, i, j, elem_l - elem_r);
        }
    }

    return new_matrix;
}

static double find_elem(const Matrix *l, const Matrix *r, size_t x, size_t y) {
    size_t cols_l = 0;
    get_cols(l, &cols_l);
    double s = 0;

    double elem_l = 0;
    double elem_r = 0;

    for (size_t i = 0; i < cols_l; i++) {
        get_elem(l, x, i, &elem_l);
        get_elem(r, i, y, &elem_r);
        s += elem_l * elem_r;
    }

    return s;
}

Matrix *mul(const Matrix *l, const Matrix *r) {
    size_t rows_l = 0;
    size_t cols_l = 0;
    size_t rows_r = 0;
    size_t cols_r = 0;

    int rc = get_rows(l, &rows_l);
    rc += get_cols(l, &cols_l);
    rc += get_rows(r, &rows_r);
    rc += get_cols(r, &cols_r);
    if (cols_l != rows_r || rc != 0) {
        return NULL;
    }

    Matrix *new_matrix = create_matrix(rows_l, cols_r);

    double new_elem;

    for (size_t i = 0; i < rows_l; i++) {
        for (size_t j = 0; j < cols_r; j++) {
            new_elem = find_elem(l, r, i, j);
            set_elem(new_matrix, i, j, new_elem);
        }
    }

    return new_matrix;
}

static int replace_rows(Matrix *mat, int x, size_t rows) {
    double elem = 0;

    get_elem(mat, x, x, &elem);
    double max = fabs(elem);
    size_t ind = x;

    for (size_t i = x + 1; i < rows; i++) {
        get_elem(mat, i, x, &elem);
        if (fabs(elem) > max) {
            max = fabs(elem);
            ind = i;
        }
    }
    if (max == 0) {
        return 1;
    }

    get_elem(mat, x, x, &elem);
    if (max == fabs(elem)) {
        return 2;
    }

    double buff = 0;

    for (size_t i  = 0; i < rows; i++) {
        get_elem(mat, x, i, &buff);
        get_elem(mat, ind, i, &elem);
        set_elem(mat, x, i, elem);
        set_elem(mat, ind, i, buff);
    }
    return 0;
}

int det(const Matrix *matrix, double *val) {
    size_t rows = 0;
    size_t cols = 0;

    double buff;
    int minus = 1;
    *val = 1;

    int rc = get_rows(matrix, &rows);
    rc += get_cols(matrix, &cols);
    if (rc != 0 || cols != rows || cols == 0) {
        return 1;
    }
    Matrix *mat = create_matrix(rows, cols);

    double elem_1 = 0;
    double elem_2 = 0;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0 ; j< cols; j++) {
            get_elem(matrix, i, j, &elem_1);
            set_elem(mat, i, j, elem_1);
        }
    }
    for (size_t i = 0; i < rows - 1; i++) {
        rc = replace_rows(mat, i, rows);
        if (rc == 1) {
            free_matrix(mat);
            *val = 0;
            return 1;
        } else if (rc == 0) {
            minus *= -1;
        }
        for (size_t j = i + 1; j < rows; j++) {
            get_elem(mat, j, i, &elem_1);
            get_elem(mat, i, i, &elem_2);
            buff = elem_1 / elem_2;
            for (size_t k = i; k < cols; k++) {
                get_elem(mat, j, k, &elem_1);
                get_elem(mat, i, k, &elem_2);
                elem_1 -= buff * elem_2;
                set_elem(mat, j, k, elem_1);
            }
        }
    }
    for (size_t i = 0; i < rows; i++) {
        get_elem(mat, i, i, &elem_1);
        *val *= elem_1;
    }
    *val *= minus;
    free_matrix(mat);
    return 0;
}

static void del_row_and_col(const Matrix *matrix, Matrix *buff, size_t x, size_t y) {
    size_t rows = 0;
    size_t cols = 0;

    get_rows(matrix, &rows);
    get_cols(matrix, &cols);

    size_t rows_b = 0;
    size_t cols_b = 0;

    double elem = 0;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (i != x && j != y) {
                get_elem(matrix, i, j, &elem);
                set_elem(buff, rows_b, cols_b, elem);
                cols_b++;

                if (cols_b >= cols - 1) {
                    cols_b = 0;
                    rows_b++;
                }
            }
        }
    }
    return;
}

Matrix *adj(const Matrix *matrix) {
    size_t rows = 0;
    size_t cols = 0;

    int rc = get_rows(matrix, &rows);
    rc += get_cols(matrix, &cols);
    if (rc != 0 || cols != rows) {
        return NULL;
    }

    Matrix *new_matrix = create_matrix(rows, cols);
    Matrix *buff = create_matrix(rows - 1, cols - 1);

    double val = 0;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            del_row_and_col(matrix, buff, i, j);
            det(buff, &val);
            set_elem(new_matrix, i, j, pow(-1, i + j) * val);
        }
    }
    Matrix *mat = transp(new_matrix);

    free_matrix(buff);
    free_matrix(new_matrix);

    return mat;
}

Matrix *inv(const Matrix *matrix) {
    size_t rows = 0;
    size_t cols = 0;
    double val = 0;

    det(matrix, &val);
    val = 1 / val;

    int rc = get_rows(matrix, &rows);
    rc += get_cols(matrix, &cols);
    if (rc != 0 || cols != rows || val == 0) {
        return NULL;
    }

    Matrix *buff = adj(matrix);
    Matrix *new_matrix = mul_scalar(buff, val);

    free_matrix(buff);
    return new_matrix;
}
