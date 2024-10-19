#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows <= 0 || columns <= 0) return 1;
  result->rows = rows;
  result->columns = columns;
  result->matrix = (double **)malloc(sizeof(double *) * rows);
  for (int i = 0; i < rows; i++) {
    result->matrix[i] = (double *)malloc(sizeof(double) * columns);
  }
  for (int n = 0; n < rows; n++) {
    for (int m = 0; m < columns; m++) result->matrix[n][m] = 0;
  }
  return 0;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (A->matrix == NULL || B->matrix == NULL) return FAILURE;
  int res = SUCCESS;
  if (A->rows != B->rows || A->columns != B->columns) {
    res = FAILURE;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPS) {
          res = FAILURE;
          break;
        }
      }
    }
  }
  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (A->matrix == NULL || B->matrix == NULL) return 1;
  int res = 0;
  if (A->rows != B->rows || A->columns != B->columns) {
    res = 2;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (A->matrix == NULL || B->matrix == NULL) return 1;
  int res = 0;
  if (A->rows != B->rows || A->columns != B->columns) {
    res = 2;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = 0;
  if (A->matrix == NULL) {
    res = 1;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (A->matrix == NULL || B->matrix == NULL) return 1;
  int res = 0;
  if (A->columns != B->rows) {
    res = 2;
  } else {
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }
  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (A->matrix == NULL) return 1;
  s21_create_matrix(A->columns, A->rows, result);
  int res = 0;
  for (int i = 0; i < A->columns; i++) {
    for (int j = 0; j < A->rows; j++) {
      result->matrix[i][j] = A->matrix[j][i];
    }
  }
  return res;
}

int s21_determinant(matrix_t *A, double *result) {
  if (is_matrix(A)) return 1;
  if (A->rows != A->columns) {
    return 2;
  } else if (A->rows == 1) {
    *result = A->matrix[0][0];
  } else if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    double determinant = 0;
    matrix_t minor;
    s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
    for (int k = 0; k < A->columns; k++) {
      fill_minor(A, &minor, 0, k);
      s21_determinant(&minor, &determinant);
      *result += pow(-1, k) * A->matrix[0][k] * determinant;
      determinant = 0;
    }
    s21_remove_matrix(&minor);
  }
  return 0;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  s21_create_matrix(A->rows, A->columns, result);
  if (is_matrix(A)) return 1;
  int res = 0;
  if (A->rows != A->columns) {
    res = 2;
  } else {
    double determinant = 0;
    s21_determinant(A, &determinant);
    if (!determinant)
      res = 2;
    else {
      if (A->rows == 1) {
        result->matrix[0][0] = A->matrix[0][0];
      } else if (A->rows == 2) {
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 2; j++) {
            result->matrix[i][j] = pow(-1, i + j) * A->matrix[!i][!j];
          }
        }
      } else {
        matrix_t minor;
        s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            determinant = 0;
            fill_minor(A, &minor, i, j);
            s21_determinant(&minor, &determinant);
            result->matrix[i][j] = pow(-1, i + j) * determinant;
          }
        }
        s21_remove_matrix(&minor);
      }
    }
  }
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (is_matrix(A)) {
    s21_create_matrix(A->rows, A->columns, result);
    return 1;
  }
  if (A->rows != A->columns) {
    s21_create_matrix(A->rows, A->columns, result);
    return 2;
  }
  int res = 0;
  if (A->rows == 1) {
    s21_create_matrix(A->rows, A->columns, result);
    result->matrix[0][0] = 1.0 / A->matrix[0][0];
  } else {
    double d = 0;
    s21_determinant(A, &d);
    if (!d) {
      res = 2;
    } else {
      matrix_t tmp_res;
      matrix_t M;
      s21_calc_complements(A, &tmp_res);
      s21_transpose(&tmp_res, &M);
      s21_mult_number(&M, 1.0 / d, result);
      s21_remove_matrix(&tmp_res);
      s21_remove_matrix(&M);
    }
  }
  return res;
}

void fill_minor(matrix_t *A, matrix_t *M, int i, int j) {
  int m = 0;
  int n = 0;
  for (int r = 0; r < M->rows; r++) {
    if (r == i) m++;
    for (int c = 0; c < M->columns; c++) {
      if (c == j) n++;
      M->matrix[r][c] = A->matrix[m][n++];
    }
    n = 0;
    m++;
  }
}

int is_matrix(matrix_t *A) {
  int res = 0;
  if (A->matrix == NULL || !A->columns || !A->rows) res = 1;
  return res;
}