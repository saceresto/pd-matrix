#include "m_pd.h"

static t_class *mtx_mult_class;

typedef struct _mtx_mult {
    t_object x_obj;
    int rows_B;
    int cols_B;
    int size_B;
    t_float *B;
    int B_set;
    t_outlet *out_matrix; 
    t_outlet *out_error;  
} t_mtx_mult;

static int check_matrix(int argc, t_atom *argv, int *rows, int *cols) {
    if (argc < 2) return 0;
    *rows = (int)atom_getfloat(&argv[0]);
    *cols = (int)atom_getfloat(&argv[1]);
    if (*rows <= 0 || *cols <= 0) return 0;
    if (argc - 2 != (*rows) * (*cols)) return 0;
    return 1;
}

void mtx_mult_list_B(t_mtx_mult *x, t_symbol *s, int argc, t_atom *argv) {
    int rows, cols;
    if (!check_matrix(argc, argv, &rows, &cols)) {
        outlet_bang(x->out_error);
        return;
    }

    int size = rows * cols;
    if (x->B) freebytes(x->B, sizeof(t_float) * x->size_B);

    x->rows_B = rows;
    x->cols_B = cols;
    x->size_B = size;
    x->B = (t_float *)getbytes(sizeof(t_float) * size);

    for (int i = 0; i < size; i++) {
        x->B[i] = atom_getfloat(&argv[i + 2]);
    }
    x->B_set = 1;
}

void mtx_mult_list(t_mtx_mult *x, t_symbol *s, int argc, t_atom *argv) {
    int rows_A, cols_A;
    if (!check_matrix(argc, argv, &rows_A, &cols_A)) {
        outlet_bang(x->out_error);
        return;
    }

    if (!x->B_set || cols_A != x->rows_B) {
        pd_error(x, "mtx_mult: dimension mismatch or B not set");
        outlet_bang(x->out_error);
        return;
    }

    int out_rows = rows_A;
    int out_cols = x->cols_B;
    int out_argc = (out_rows * out_cols) + 2;
    
    t_atom *out = (t_atom *)getbytes(sizeof(t_atom) * out_argc);
    SETFLOAT(&out[0], out_rows);
    SETFLOAT(&out[1], out_cols);

    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < out_cols; j++) {
            t_float sum = 0.0;
            for (int k = 0; k < cols_A; k++) {
                t_float val_A = atom_getfloat(&argv[2 + (i * cols_A) + k]);
                t_float val_B = x->B[(k * out_cols) + j];
                sum += val_A * val_B;
            }
            SETFLOAT(&out[2 + (i * out_cols) + j], sum);
        }
    }

    outlet_list(x->out_matrix, &s_list, out_argc, out);
    freebytes(out, sizeof(t_atom) * out_argc);
}

void *mtx_mult_new(void) {
    t_mtx_mult *x = (t_mtx_mult *)pd_new(mtx_mult_class);
    x->B = NULL;
    x->size_B = 0;
    x->B_set = 0;
    
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("list_B"));
    x->out_matrix = outlet_new(&x->x_obj, &s_list);
    x->out_error  = outlet_new(&x->x_obj, &s_bang);
    return x;
}

void mtx_mult_free(t_mtx_mult *x) {
    if (x->B) freebytes(x->B, sizeof(t_float) * x->size_B);
}

void mtx_mult_setup(void) {
    mtx_mult_class = class_new(gensym("mtx_mult"), (t_newmethod)mtx_mult_new,
        (t_method)mtx_mult_free, sizeof(t_mtx_mult), CLASS_DEFAULT, 0);
    class_addlist(mtx_mult_class, mtx_mult_list);
    class_addmethod(mtx_mult_class, (t_method)mtx_mult_list_B, gensym("list_B"), A_GIMME, 0);
}
