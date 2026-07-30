#ifndef MATRIX_H
#define MATRIX_H

#include "m_pd.h"

typedef struct _matrix {
    int rows;
    int cols;
    double *data;
} t_matrix;

t_matrix *matrix_create(int rows, int cols);
void matrix_free(t_matrix *m);

int mtx_validate_list(const char *obj_name, int argc, t_atom *argv, int *rows, int *cols);

#endif
