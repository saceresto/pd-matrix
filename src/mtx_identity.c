#include "m_pd.h"
#include <stdlib.h>

static t_class *mtx_identity_class;

typedef struct _mtx_identity {
    t_object x_obj;
    t_outlet *out_matrix;
    int dim;
} t_mtx_identity;

static void mtx_identity_bang(t_mtx_identity *x) {
    int dim = x->dim;
    if (dim < 1) dim = 4; 

    int total_atoms = 2 + (dim * dim);
    t_atom *out = (t_atom *)getbytes(sizeof(t_atom) * total_atoms);

    SETFLOAT(&out[0], (t_float)dim);
    SETFLOAT(&out[1], (t_float)dim);

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            int idx = 2 + (i * dim + j);
            SETFLOAT(&out[idx], (i == j) ? 1.0f : 0.0f);
        }
    }

    outlet_list(x->out_matrix, &s_list, total_atoms, out);
    freebytes(out, sizeof(t_atom) * total_atoms);
}

static void mtx_identity_float(t_mtx_identity *x, t_floatarg f) {
    int dim = (int)f;
    if (dim > 0) {
        x->dim = dim;
        mtx_identity_bang(x);
    }
}

static void *mtx_identity_new(t_floatarg f) {
    t_mtx_identity *x = (t_mtx_identity *)pd_new(mtx_identity_class);
    x->dim = (f > 0) ? (int)f : 4; 
    x->out_matrix = outlet_new(&x->x_obj, &s_list);
    return x;
}

void mtx_identity_setup(void) {
    mtx_identity_class = class_new(gensym("mtx_identity"),
        (t_newmethod)mtx_identity_new,
        0,
        sizeof(t_mtx_identity),
        CLASS_DEFAULT,
        A_DEFFLOAT,
        0);

    class_addbang(mtx_identity_class, mtx_identity_bang);
    class_addfloat(mtx_identity_class, mtx_identity_float);
}
