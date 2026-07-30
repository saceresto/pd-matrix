#include "m_pd.h"
#include <stdlib.h>
#include <time.h>

static t_class *mtx_random_class;

typedef struct _mtx_random {
    t_object x_obj;
    int rows;
    int cols;
    t_outlet *outlet;
} t_mtx_random;

void mtx_random_bang(t_mtx_random *x)
{
    int size = x->rows * x->cols;
    int argc = size + 2;

    t_atom *argv = (t_atom *)getbytes(sizeof(t_atom) * argc);

    SETFLOAT(&argv[0], x->rows);
    SETFLOAT(&argv[1], x->cols);

    for(int i = 0; i < size; i++)
    {
        double value = (double)rand() / (double)RAND_MAX;
        SETFLOAT(&argv[i + 2], value);
    }

    outlet_list(x->outlet, &s_list, argc, argv);
    freebytes(argv, sizeof(t_atom) * argc);
}

void *mtx_random_new(t_floatarg rows, t_floatarg cols)
{
    t_mtx_random *x = (t_mtx_random *)pd_new(mtx_random_class);

    x->rows = (int)rows;
    x->cols = (int)cols;

    x->outlet = outlet_new(&x->x_obj, &s_list);

    return x;
}

void mtx_random_setup(void)
{
    mtx_random_class = class_new(
        gensym("mtx_random"),
        (t_newmethod)mtx_random_new,
        0,
        sizeof(t_mtx_random),
        CLASS_DEFAULT,
        A_DEFFLOAT,
        A_DEFFLOAT,
        0
    );

    class_addbang(mtx_random_class, mtx_random_bang);

    srand((unsigned int)time(NULL));
}
