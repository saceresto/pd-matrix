#include "m_pd.h"


static t_class *mtx_identity_class;


typedef struct _mtx_identity {

    t_object x_obj;

    int size;

    t_outlet *outlet;

} t_mtx_identity;



void mtx_identity_bang(t_mtx_identity *x)
{

    int n = x->size;


    int argc = n*n + 2;


    t_atom *argv = getbytes(
        sizeof(t_atom) * argc
    );


    SETFLOAT(&argv[0], n);
    SETFLOAT(&argv[1], n);



    for(int i = 0; i < n*n; i++)
    {

        if(i / n == i % n)
        {
            SETFLOAT(
                &argv[i+2],
                1
            );
        }
        else
        {
            SETFLOAT(
                &argv[i+2],
                0
            );
        }

    }


    outlet_list(
        x->outlet,
        &s_list,
        argc,
        argv
    );


    freebytes(
        argv,
        sizeof(t_atom) * argc
    );

}



void *mtx_identity_new(t_floatarg size)
{

    t_mtx_identity *x;


    x = (t_mtx_identity *)pd_new(
        mtx_identity_class
    );


    x->size = size;


    x->outlet = outlet_new(
        &x->x_obj,
        &s_list
    );


    return x;

}




void mtx_identity_setup(void)
{

    mtx_identity_class = class_new(
        gensym("mtx_identity"),
        (t_newmethod)mtx_identity_new,
        0,
        sizeof(t_mtx_identity),
        CLASS_DEFAULT,
        A_DEFFLOAT,
        0
    );


    class_addbang(
        mtx_identity_class,
        mtx_identity_bang
    );

}
