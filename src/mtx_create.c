#include "m_pd.h"
#include "matrix.h"


static t_class *mtx_create_class;


typedef struct _mtx_create {

    t_object x_obj;

    int rows;
    int cols;

    t_outlet *outlet;

} t_mtx_create;



void mtx_create_list(t_mtx_create *x,
                     t_symbol *s,
                     int argc,
                     t_atom *argv)
{

    int expected = x->rows * x->cols;


    if (argc != expected)
    {
        post(
            "mtx_create: expected %d values, got %d",
            expected,
            argc
        );

        return;
    }



    int out_argc = expected + 2;


    t_atom *out_argv = getbytes(
        sizeof(t_atom) * out_argc
    );



    // rows
    SETFLOAT(
        &out_argv[0],
        x->rows
    );


    // cols
    SETFLOAT(
        &out_argv[1],
        x->cols
    );



    // copy values
    for (int i = 0; i < expected; i++)
    {

        SETFLOAT(
            &out_argv[i + 2],
            atom_getfloat(&argv[i])
        );

    }



    outlet_list(
        x->outlet,
        &s_list,
        out_argc,
        out_argv
    );



    freebytes(
        out_argv,
        sizeof(t_atom) * out_argc
    );

}




void *mtx_create_new(t_floatarg rows,
                     t_floatarg cols)
{

    t_mtx_create *x;


    x = (t_mtx_create *)pd_new(
        mtx_create_class
    );


    x->rows = rows;
    x->cols = cols;



    x->outlet = outlet_new(
        &x->x_obj,
        &s_list
    );


    return (void *)x;

}




void mtx_create_setup(void)
{

    mtx_create_class = class_new(
        gensym("mtx_create"),
        (t_newmethod)mtx_create_new,
        0,
        sizeof(t_mtx_create),
        CLASS_DEFAULT,
        A_DEFFLOAT,
        A_DEFFLOAT,
        0
    );


    class_addlist(
        mtx_create_class,
        mtx_create_list
    );

}
