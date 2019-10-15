#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

Matrix* create_matrix(size_t rows, size_t cols) {
    Matrix *matrix = malloc(sizeof(Matrix));
    size_t i;

    double *a_new = calloc(rows * cols, sizeof(double));
    double **a_p_new = malloc(rows * sizeof(double*));

    if (a_new && a_p_new) {
        matrix->a = a_new;
        matrix->a_p = a_p_new;
        a_new = NULL;
        a_p_new = NULL;
    } else {
        free_matrix(matrix);
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        matrix->a_p[i] = matrix->a + i * cols;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    return matrix;
}

Matrix* create_matrix_from_file(const char* path_file) {
    Matrix *matrix = malloc(sizeof(Matrix));
    size_t i, j;

    FILE *f = fopen(path_file, "r");
    if (!f) {
        printf("Can not open file: %s\n", path_file);
        free_matrix(matrix);
        return NULL;
    }

    fscanf(f, "%zu %zu", &matrix->rows, &matrix->cols);

    double *a_new = calloc(matrix->rows * matrix->cols, sizeof(double));
    double **a_p_new = calloc(matrix->rows, sizeof(double*));
    if (a_new && a_p_new) {
        matrix->a = a_new;
        matrix->a_p = a_p_new;
        a_new = NULL;
        a_p_new = NULL;
    } else {
        fclose(f);
        free_matrix(matrix);
        return NULL;
    }

    for (i = 0; i < matrix->rows; i++) {
        matrix->a_p[i] = matrix->a + i * matrix->cols;
    }

    for (i = 0; i < matrix->rows; i++) {
        for (j = 0; j < matrix->cols; j++) {
            fscanf(f, "%lf", &matrix->a_p[i][j]);
        }
    }

    fclose(f);
    return matrix;
}

int free_matrix(Matrix* matrix) {
    free(matrix->a);
    free(matrix->a_p);
    free(matrix);

    return 0;
}

int get_rows(const Matrix* matrix, size_t* rows) {
    if (!matrix) {
        return 1;
    }

    *rows = matrix->rows;
    return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (!matrix) {
        return 1;
    }

    *cols = matrix->cols;
    return 0;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (!matrix) {
        return 1;
    }

    *val = matrix->a_p[row][col];
    return 0;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (!matrix) {
        return 1;
    }

    matrix->a_p[row][col] = val;
    return 0;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
    size_t rows = 0;
    size_t cols = 0;

    int rc = get_rows(matrix, &rows);
    rc += get_cols(matrix, &cols);
    if (rc != 0 || rows == 0 || cols == 0) {
        return NULL;
    }

    Matrix *new_matrix = create_matrix(rows, cols);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            new_matrix->a_p[i][j] = matrix->a_p[i][j] * val;
        }
    }
    return new_matrix;
}

Matrix* transp(const Matrix* matrix) {
    size_t rows = 0;
    size_t cols = 0;

    int rc = get_rows(matrix, &rows);
    rc += get_cols(matrix, &cols);
    if (rc != 0 || rows == 0 || cols == 0) {
        return NULL;
    }

    Matrix *new_matrix = create_matrix(cols, rows);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            new_matrix->a_p[j][i] = matrix->a_p[i][j];
        }
    }

    return new_matrix;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
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

    for (size_t i = 0; i < rows_l; i++) {
        for (size_t j = 0; j < cols_l; j++) {
            new_matrix->a_p[i][j] = l->a_p[i][j] + r->a_p[i][j];
        }
    }

    return new_matrix;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
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

    for (size_t i = 0; i < rows_l; i++) {
        for (size_t j = 0; j < cols_l; j++) {
            new_matrix->a_p[i][j] = l->a_p[i][j] - r->a_p[i][j];
        }
    }

    return new_matrix;
}

double find_elem(const Matrix* l, const Matrix* r, size_t x, size_t y) {
    size_t cols_l = 0;
    get_cols(l, &cols_l);
    double s = 0;

    for (size_t i = 0; i < cols_l; i++) {
        s += l->a_p[x][i] * r->a_p[i][y];
    }

    return s;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    size_t i;
    size_t j;

    size_t rows_l = 0;
    size_t cols_l = 0;
    size_t rows_r = 0;
    size_t cols_r = 0;

    int rc = get_rows(l, &rows_l);
    rc += get_cols(l, &cols_l);
    rc += get_rows(r, &rows_r);
    rc += get_cols(r, &cols_r);
    if (cols_l != rows_r || rc != 0)
        return NULL;

    Matrix *new_matrix = create_matrix(rows_l, cols_r);

    for (i = 0; i < rows_l; i++) {
        for (j = 0; j < cols_r; j++) {
            new_matrix->a_p[i][j] = find_elem(l, r, i, j);
        }
    }

    return new_matrix;
}

int replace_rows(Matrix* mat, int x, size_t rows) {
    size_t i;
    size_t j;

    double max = fabs(mat->a_p[x][x]);
    size_t ind = x;
    for (i = x + 1; i < rows; i++) {
        if (fabs(mat->a_p[i][x]) > max) {
            max = fabs(mat->a_p[i][x]);
            ind = i;
        }
    }
    if (max == 0) {
        return 1;
    }
    if (max == fabs(mat->a_p[x][x])) {
        return 2;
    }
    for (j = 0; j < rows; j++) {
        double buff = mat->a_p[x][j];
        mat->a_p[x][j] = mat->a_p[ind][j];
        mat->a_p[ind][j] = buff;
    }
    return 0;
}

int det(const Matrix* matrix, double* val) {
    size_t i;
    size_t j;
    size_t k;
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
    for (i = 0; i < rows; i++) {
        for (j = 0 ; j< cols; j++) {
            mat->a_p[i][j] = matrix->a_p[i][j];
        }
    }
    for (i = 0; i < rows - 1; i++) {
        rc = replace_rows(mat, i, rows);
        if (rc == 1) {
            free_matrix(mat);
            *val = 0;
            return 1;
        } else if (rc == 0) {
            minus *= -1;
        }
        for (j = i + 1; j < rows; j++) {
            buff = mat->a_p[j][i] / mat->a_p[i][i];
            for (k = i; k < cols; k++) {
                mat->a_p[j][k] -= buff * mat->a_p[i][k];
            }
        }
    }
    for (i = 0; i < rows; i++) {
        *val *= mat->a_p[i][i];
    }
    *val *= minus;
    free_matrix(mat);
    return 0;
}

void del_row_and_col(const Matrix* matrix, Matrix* buff, size_t x, size_t y) {
    size_t rows = 0;
    size_t cols = 0;

    get_rows(matrix, &rows);
    get_cols(matrix, &cols);

    size_t rows_b = 0;
    size_t cols_b = 0;
    size_t i;
    size_t j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (i != x && j != y) {
                buff->a_p[rows_b][cols_b] = matrix->a_p[i][j];
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

Matrix* adj(const Matrix* matrix) {
    size_t i;
    size_t j;
    size_t rows = 0;
    size_t cols = 0;
    double val = 0;

    int rc = get_rows(matrix, &rows);
    rc += get_cols(matrix, &cols);
    if (rc != 0 || cols != rows) {
        return NULL;
    }

    Matrix *new_matrix = create_matrix(rows, cols);
    Matrix *buff = create_matrix(rows - 1, cols - 1);

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            del_row_and_col(matrix, buff, i, j);
            det(buff, &val);
            new_matrix->a_p[i][j] = pow(-1, i + j) * val;
        }
    }
    Matrix *mat = transp(new_matrix);

    free_matrix(buff);
    free_matrix(new_matrix);

    return mat;
}

Matrix* inv(const Matrix* matrix) {
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
