#include "m_pd.h"


static t_class *hello_class;


void hello_bang(void *x)
{
    post("hello from pd-matrix");
}


void *hello_new(void)
{
    return pd_new(hello_class);
}


void hello_setup(void)
{
    hello_class = class_new(
        gensym("hello"),
        (t_newmethod)hello_new,
        0,
        sizeof(t_object),
        CLASS_DEFAULT,
        0
    );

    class_addbang(
        hello_class,
        hello_bang
    );
}
