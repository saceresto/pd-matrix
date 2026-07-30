#include "m_pd.h"
#include "matrix.h"


static t_class *mtx_store_class;


typedef struct _mtx_store {

    t_object x_obj;

    t_matrix *matrix;

    t_outlet *outlet;

} t_mtx_store;


void mtx_store_free(t_mtx_store *x) {
    if (x->matrix != NULL) {
        matrix_free(x->matrix);
    }
}

void mtx_store_list(t_mtx_store *x,
                    t_symbol *s,
                    int argc,
                    t_atom *argv)
{

    if (argc < 2)
    {
        post("mtx_store: need rows and cols");
        return;
    }


    int rows = (int)atom_getfloat(argv);
    int cols = (int)atom_getfloat(argv + 1);


    int expected = rows * cols;


    if (argc - 2 != expected)
    {
        post("mtx_store: wrong number of elements");
        post("expected %d values, got %d",
             expected,
             argc - 2);
        return;
    }



    if (x->matrix != NULL)
    {
        matrix_free(x->matrix);
    }



    x->matrix = matrix_create(rows, cols);



    int index = 2;


    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {

            x->matrix->data[i * cols + j] =
                atom_getfloat(argv + index);

            index++;

        }
    }


    post("stored matrix: %d x %d", rows, cols);



    outlet_list(
        x->outlet,
        &s_list,
        argc,
        argv
    );

}




void *mtx_store_new(void)
{
    t_mtx_store *x;


    x = (t_mtx_store *)pd_new(mtx_store_class);


    x->matrix = NULL;



    x->outlet = outlet_new(
        &x->x_obj,
        &s_list
    );


    return (void *)x;
}





void mtx_store_setup(void)
{

    mtx_store_class = class_new(
        gensym("mtx_store"),
        (t_newmethod)mtx_store_new,
        (t_method)mtx_store_free,
        sizeof(t_mtx_store),
        CLASS_DEFAULT,
        0
    );


    class_addlist(
        mtx_store_class,
        mtx_store_list
    );

}
