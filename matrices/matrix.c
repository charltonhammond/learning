#include <stdio.h>
#include <stdlib.h>

/* matrix structure*/
typedef struct {
    int n_col;
    int n_row;
    double *data;
} matrix_t;

/* Macro to more intuitivly access a matrix entry */
#define MATRIX_ELEMET(A, col, row) A->data[row * A->n_col + col]

matrix_t* construct_matrix(int n_col, int n_row) {
    matrix_t *A = malloc(sizeof(matrix_t));
    A->n_col = n_col;
    A->n_row = n_row;
    A->data = calloc(n_col*n_row, sizeof(double));
    
    return A;
}

void free_matrix(matrix_t *A) {
    if (A) {
        free(A->data);
        free(A);
    }
}

void print_matrix(matrix_t *A) {
    printf("[ \n");
    for (int j = 0; j < A->n_row; j++) {
        for (int i = 0; i < A->n_col; i++) {
            printf("  %.1f ", MATRIX_ELEMET(A, i, j)); 
        }
        printf("\n");
    }
    printf("]\n");
}

matrix_t* add_matrix(matrix_t *A, matrix_t *B) {
    /* First check the matrices are the same sie, if not, refuse to do it */
    if ((A->n_col != B->n_col) || (A->n_row != B->n_row)) {
        printf("Matrices do not have matching dimentions, additin refused.\n");
        return NULL;
    }

    matrix_t *sol = construct_matrix(A->n_col, A->n_row);
    for (int i = 0; i < A->n_col; i++) {
        for (int j = 0; j < A->n_row; j++) {
            MATRIX_ELEMET(sol, i, j) = MATRIX_ELEMET(A, i, j) + MATRIX_ELEMET(B, i, j);
        }
    }

    return sol;
}

matrix_t* scalar_multiply(double s, matrix_t *A) {
    matrix_t *sol = construct_matrix(A->n_col, A->n_row);
    for (int i = 0; i < A->n_col; i++) {
        for (int j = 0; j < A->n_row; j++) {
            MATRIX_ELEMET(sol, i, j) = s * MATRIX_ELEMET(A, i, j);
        }
    }

    return sol;
}

/* Multiplying matrices requires a bit of overhead work to actually do it quick
 * - Matrix multiplation is a memory constrained task, this means that most of 
 *   time is spent shuffling memory about and not actually doing any math.
 * - We want to optimise memory acessing, cash usage, and parallel processing.
 * */
matrix_t* multiply_matrix(matrix_t *A, matrix_t *B) {
    /* First check the matrices are compatable, if not, refuse to do it */
    if ((A->n_col != B->n_row) || (A->n_row != B->n_col)) {
        printf("Matrices are not compatable, multiplication refused.\n");
        return NULL;
    }

    matrix_t *sol = construct_matrix(B->n_col, A->n_row);
    
    return sol;
}

/* Naive apprpach */
matrix_t* naive_multiply_matrix(matrix_t *A, matrix_t *B) {
    /* First check the matrices are compatable, if not, refuse to do it */
    if (A->n_col != B->n_row) {
        printf("Matrices are not compatable, multiplication refused.\n");
        return NULL;
    }

    matrix_t *sol = construct_matrix(B->n_col, A->n_row);
    for (int i = 0; i < sol->n_col; i++) {
        for (int j = 0; j < sol->n_row; j++) {
            for (int k = 0; k < A->n_col; k++) {
                MATRIX_ELEMET(sol, i, j) += MATRIX_ELEMET(A, k, j) * MATRIX_ELEMET(B, i, k);
            }
        }
    }

    return sol;
}

matrix_t* transpose(matrix_t *A) {
    matrix_t *sol = construct_matrix(A->n_row, A->n_col);

    #pragma omp parallel for
    for (int i = 0; i < A->n_row; i++) {
        for (int j = 0; j < A->n_col; j++) {
            MATRIX_ELEMET(sol, j, i) = MATRIX_ELEMET(A, i, j);
        }
    }

    return sol;
}

matrix_t* gaussian_elim() {
    return NULL;
}

int main() {
    matrix_t *A = construct_matrix(2, 2);
    matrix_t *B = construct_matrix(2, 2);
    
    MATRIX_ELEMET(A, 1, 0) = 2.1;
    MATRIX_ELEMET(B, 0, 1) = 42.5;

    print_matrix(A);
    print_matrix(B);

    A = transpose(A);
    A = add_matrix(A, B);
    A = scalar_multiply(2, A);

    print_matrix(A);

    return 0;
}
