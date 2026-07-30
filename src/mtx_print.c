#include "m_pd.h"
#include "matrix.h"
#include <stdio.h>
#include <string.h>

static t_class *mtx_print_class;


typedef struct _mtx_print {

    t_object x_obj;

} t_mtx_print;



void mtx_print_list(t_mtx_print *x,
                    t_symbol *s,
                    int argc,
                    t_atom *argv)
{

    if (argc < 2)
    {
        post("mtx_print: need rows and cols");
        return;
    }


    int rows = (int)atom_getfloat(argv);
    int cols = (int)atom_getfloat(argv + 1);


    int expected = rows * cols;


    if (argc - 2 != expected)
    {
        post("mtx_print: wrong number of elements");
        post("expected %d values, got %d",
             expected,
             argc - 2);
        return;
    }


    post("matrix %d x %d:",
         rows,
         cols);


    int index = 2;


    for (int i = 0; i < rows; i++)
    {

        char line[1024] = "";


        for (int j = 0; j < cols; j++)
        {

            char temp[64];


            sprintf(
                temp,
                "%g ",
                atom_getfloat(argv + index)
            );


            strcat(line, temp);


            index++;

        }


        post("%s", line);

    }

}



void *mtx_print_new(void)
{
    t_mtx_print *x;


    x = (t_mtx_print *)pd_new(mtx_print_class);


    return (void *)x;
}




void mtx_print_setup(void)
{

    mtx_print_class = class_new(
        gensym("mtx_print"),
        (t_newmethod)mtx_print_new,
        0,
        sizeof(t_mtx_print),
        CLASS_DEFAULT,
        0
    );


    class_addlist(
        mtx_print_class,
        mtx_print_list
    );

}
