#include "m_pd.h"
#include "matrix.h"

static t_class *mtx_add_class;

typedef struct _mtx_add {
    t_object x_obj;

    int rows;
    int cols;
    int size;

    t_float *A;
    t_float *B;

    int B_set;

    t_outlet *out_matrix; 
    t_outlet *out_error;  
} t_mtx_add;

// Right
void mtx_add_list_B(t_mtx_add *x, t_symbol *s, int argc, t_atom *argv) {
    int rows, cols;
    if (!mtx_validate_list("mtx_add (right inlet)", argc, argv, &rows, &cols)) {
        outlet_bang(x->out_error);
        return;
    }

    int size = rows * cols;

    if (x->B) {
        freebytes(x->B, sizeof(t_float) * x->size);
    }

    x->rows = rows;
    x->cols = cols;
    x->size = size;
    x->B = (t_float *)getbytes(sizeof(t_float) * size);

    for (int i = 0; i < size; i++) {
        x->B[i] = atom_getfloat(&argv[i + 2]);
    }

    x->B_set = 1;
}

// Left
void mtx_add_list(t_mtx_add *x, t_symbol *s, int argc, t_atom *argv) {
    int rows, cols;
    if (!mtx_validate_list("mtx_add (left inlet)", argc, argv, &rows, &cols)) {
        outlet_bang(x->out_error);
        return;
    }

    if (!x->B_set) {
        pd_error(x, "mtx_add: matrix B is not set");
        outlet_bang(x->out_error);
        return;
    }

    if (rows != x->rows || cols != x->cols) {
        pd_error(x, "mtx_add: dimension mismatch (%dx%d vs %dx%d)", 
                 rows, cols, x->rows, x->cols);
        outlet_bang(x->out_error);
        return;
    }

    int size = rows * cols;
    int out_argc = size + 2;
    t_atom *out = (t_atom *)getbytes(sizeof(t_atom) * out_argc);

    SETFLOAT(&out[0], rows);
    SETFLOAT(&out[1], cols);

    for (int i = 0; i < size; i++) {
        t_float val_A = atom_getfloat(&argv[i + 2]);
        SETFLOAT(&out[i + 2], val_A + x->B[i]);
    }

    outlet_list(x->out_matrix, &s_list, out_argc, out);
    freebytes(out, sizeof(t_atom) * out_argc);
}

void *mtx_add_new(void) {
    t_mtx_add *x = (t_mtx_add *)pd_new(mtx_add_class);

    x->A = NULL;
    x->B = NULL;
    x->size = 0;
    x->B_set = 0;

    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("list_B"));

    x->out_matrix = outlet_new(&x->x_obj, &s_list);
    x->out_error  = outlet_new(&x->x_obj, &s_bang); // Outputs bang on error

    return x;
}

void mtx_add_free(t_mtx_add *x) {
    if (x->B) {
        freebytes(x->B, sizeof(t_float) * x->size);
    }
}

void mtx_add_setup(void) {
    mtx_add_class = class_new(
        gensym("mtx_add"),
        (t_newmethod)mtx_add_new,
        (t_method)mtx_add_free,
        sizeof(t_mtx_add),
        CLASS_DEFAULT,
        0
    );

    class_addlist(mtx_add_class, mtx_add_list);
    class_addmethod(mtx_add_class, (t_method)mtx_add_list_B, gensym("list_B"), A_GIMME, 0);
}
