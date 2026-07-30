#include <stdlib.h>
#include <string.h>
#include "matrix.h"



t_matrix *matrix_create(int rows, int cols)
{

    t_matrix *m;


    m = malloc(sizeof(t_matrix));


    if (m == NULL)
    {
        return NULL;
    }


    m->rows = rows;
    m->cols = cols;


    m->data = malloc(
        sizeof(double) * rows * cols
    );


    if (m->data == NULL)
    {
        free(m);
        return NULL;
    }


    memset(
        m->data,
        0,
        sizeof(double) * rows * cols
    );


    return m;

}




void matrix_free(t_matrix *m)
{

    if (m == NULL)
    {
        return;
    }


    free(m->data);

    free(m);

}
