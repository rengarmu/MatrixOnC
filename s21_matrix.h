#ifndef S21_MATRIX
#define S21_MATRIX

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define FAILURE 0
#define SUCCESS 1

#define EPS 1E-7

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns,
                      matrix_t *result);      // создание матриц
void s21_remove_matrix(matrix_t *A);          // очистка матриц
int s21_eq_matrix(matrix_t *A, matrix_t *B);  // сравнение матриц

int s21_sum_matrix(matrix_t *A, matrix_t *B,
                   matrix_t *result);  // сложение матриц
int s21_sub_matrix(matrix_t *A, matrix_t *B,
                   matrix_t *result);  // вычитание  матриц
int s21_mult_number(matrix_t *A, double number,
                    matrix_t *result);  // умножение матрицы на число
int s21_mult_matrix(matrix_t *A, matrix_t *B,
                    matrix_t *result);  // умножение двух матриц
int s21_transpose(matrix_t *A, matrix_t *result);  // транспонирование матрицы
int s21_calc_complements(
    matrix_t *A,
    matrix_t *result);  // минор матрицы и матрица алгебраических дополнений
int s21_determinant(matrix_t *A, double *result);  // определитель матрицы
int s21_inverse_matrix(matrix_t *A, matrix_t *result);  // обратная матрица

void fill_minor(matrix_t *A, matrix_t *M, int i, int j);
int is_matrix(matrix_t *A);

#endif
