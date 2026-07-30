#include "m_pd.h"


static t_class *mtx_transpose_class;


typedef struct _mtx_transpose {

    t_object x_obj;

    t_outlet *outlet;

} t_mtx_transpose;



void mtx_transpose_list(t_mtx_transpose *x,
                        t_symbol *s,
                        int argc,
                        t_atom *argv)
{

    if(argc < 2)
    {
        post("mtx_transpose: missing dimensions");
        return;
    }


    int rows = atom_getfloat(argv);
    int cols = atom_getfloat(argv + 1);


    int values = rows * cols;


    if(argc - 2 != values)
    {
        post("mtx_transpose: wrong number of values");
        return;
    }



    int out_argc = argc;


    t_atom *out =
        getbytes(sizeof(t_atom) * out_argc);



    SETFLOAT(
        &out[0],
        cols
    );


    SETFLOAT(
        &out[1],
        rows
    );



    for(int i = 0; i < rows; i++)
    {

        for(int j = 0; j < cols; j++)
        {

            int old_index =
                i * cols + j;


            int new_index =
                j * rows + i;


            SETFLOAT(
                &out[new_index + 2],
                atom_getfloat(
                    &argv[old_index + 2]
                )
            );

        }

    }



    outlet_list(
        x->outlet,
        &s_list,
        out_argc,
        out
    );



    freebytes(
        out,
        sizeof(t_atom) * out_argc
    );

}




void *mtx_transpose_new(void)
{

    t_mtx_transpose *x;


    x = (t_mtx_transpose *)pd_new(
        mtx_transpose_class
    );


    x->outlet = outlet_new(
        &x->x_obj,
        &s_list
    );


    return x;

}




void mtx_transpose_setup(void)
{

    mtx_transpose_class = class_new(
        gensym("mtx_transpose"),
        (t_newmethod)mtx_transpose_new,
        0,
        sizeof(t_mtx_transpose),
        CLASS_DEFAULT,
        0
    );


    class_addlist(
        mtx_transpose_class,
        mtx_transpose_list
    );

}
