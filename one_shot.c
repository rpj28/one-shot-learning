#include <stdio.h>
#include <stdlib.h>

double** transpose_alloc(double** train_XT_matrix, int tk, int tn) {
    train_XT_matrix = (double**)malloc(sizeof(double*)*(tk+1));
    for (int i = 0; i < tk+1; i++) {
        train_XT_matrix[i] = (double*)malloc(sizeof(double)*(tn));
    }
    return train_XT_matrix;
}

double** tranpose(double** train_X_matrix, double** train_XT_matrix, int tk, int tn) {
       
    for (int i = 0; i < tn; i++) {
        for (int j = 0; j < tk+1; j++) {
            train_XT_matrix[j][i] = train_X_matrix[i][j];
        }
    }
    return train_XT_matrix;
}

void free_tranpose(double** train_XT_matrix, int tk) {
    for (int i = 0; i < tk+1; i++) {
        free(train_XT_matrix[i]);
    }
    free(train_XT_matrix);
}

double** product_alloc(double** product, int tk) {
    product = (double**)malloc(sizeof(double*)*(tk+1));
    for (int i = 0; i < tk+1; i++) {
        product[i] = (double*)malloc(sizeof(double)*(tk+1));
    }
    return product;
}

void free_product(double** product, int tk) {
    for (int i = 0; i < tk+1; i++) {
        free(product[i]);
    }
    free(product);
}

double** identity_creation(double** inverse, int tk) {
    for (int i = 0; i < tk+1; i++) {
        for (int j = 0; j < tk+1; j++) {
            if (i == j) {
                inverse[i][j] = 1;
            } else {
                inverse[i][j] = 0;
            }
        }
    }
    return inverse;
}

double** multiply_XT_X(double** product, double** train_X_matrix, double** train_XT_matrix, int tn, int tk) {
    for (int i = 0; i < tk+1; i++) {
        for (int j = 0; j < tk+1; j++) {
            product[i][j] = 0;
            for (int k = 0; k < tn; k++) {
                product[i][j] += train_XT_matrix[i][k]*train_X_matrix[k][j];
            }
        }
    }
    return product;
}

double** multiply_inverse_XT(double** inverse, double** train_XT_matrix, double** psuedo, int tk, int tn) {
    for (int i = 0; i < tk+1; i++) {
        for (int j = 0; j < tn; j++) {
            psuedo[i][j] = 0;
            for (int k = 0; k < tk+1; k++) {
                psuedo[i][j] += inverse[i][k] * train_XT_matrix[k][j];
            }
        }
    }
    return psuedo;
}

double** multiply_psuedo_Y(double** psuedo, double** train_Y_matrix, double** W_matrix, int tk, int tn) {
    for (int i = 0; i < tk+1; i++) {
        for (int j = 0; j < 1; j++) {
            W_matrix[i][j] = 0;
            for (int k = 0; k < tn; k++) {
                W_matrix[i][j] += psuedo[i][k] * train_Y_matrix[k][j];
            }
        }
    }
    return W_matrix;
}

double** multiply_XW(double** data_X_matrix, double** W_matrix, double** data_Y_matrix, int dk, int dm) {
    for (int i = 0; i < dm; i++) {
        for (int j = 0; j < 1; j++) {
            data_Y_matrix[i][j] = 0; 
            for (int k = 0; k < dk+1; k++) {
                data_Y_matrix[i][j] += data_X_matrix[i][k] * W_matrix[k][j]; 
            }
        }
    }
    return data_Y_matrix;
}

void free_train_X_matrix(double** train_X_matrix, int tn) {
    for (int i = 0; i < tn; i++) {
        free(train_X_matrix[i]);
    }
    free(train_X_matrix);    
}

void free_train_Y_matrix(double** train_Y_matrix, int tn) {
    for (int i = 0; i < tn; i++) {
        free(train_Y_matrix[i]);
    }
    free(train_Y_matrix);
}

void free_data_X_matrix(double** data_X_matrix, int dm) {
    for (int i = 0; i < dm; i++) {
        free(data_X_matrix[i]);
    }
    free(data_X_matrix);
}

void free_data_Y_matrix(double** data_Y_matrix, int dm) {
    for (int i = 0; i < dm; i++) {
        free(data_Y_matrix[i]);
    }
    free(data_Y_matrix);
}

void free_W_matrix(double** W_matrix, int tk) {
    for (int i = 0; i < tk+1; i++) {
        free(W_matrix[i]);
    }
    free(W_matrix);
}

double** Gauss_Jordan(double** product1, double** inverse, int tk) {
    for (int i = 0; i < tk+1; i++) {
        double pivot = product1[i][i];
        if (pivot == 0) {
            printf("error");
            break;
        }
        for (int a = 0; a < tk+1; a++) {
            product1[i][a] = product1[i][a]/pivot;
            inverse[i][a] = inverse[i][a]/pivot;
            //printf("p: %f i: %f ", product1[i][a], inverse[i][a]);
        }

        for (int j = i+1; j < tk+1; j++) {
            pivot = product1[j][i];
            for (int k = 0; k < tk+1; k++) {
                product1[j][k] = product1[j][k] - (pivot*product1[i][k]);
                inverse[j][k] = inverse[j][k] - (pivot*inverse[i][k]);

            }
            
        }
    }
    for (int i = tk; i >= 0; i--) {
        for (int j = i-1; j >= 0; j--) {
            double pivot = product1[j][i];
            for (int k = 0; k < tk+1; k++) {
                product1[j][k] = product1[j][k] - (pivot*product1[i][k]);
                inverse[j][k] = inverse[j][k] - (pivot*inverse[i][k]);
            } 
        }
    }

    return inverse;
}


 
int main(int argc, char *argv[]) {
    FILE *fptrain;
    FILE *fpdata;

    char tstr[6];
    int tk;
    int tn;
    double **train_X_matrix = NULL;
    double **train_Y_matrix = NULL;
    double **W_matrix = NULL;
    double **train_XT_matrix = NULL;
    double **product = NULL;
    double **inverse = NULL;
    double **psuedo = NULL;

    char dstr[5];
    int dk;
    int dm;
    double **data_X_matrix = NULL;
    double **data_Y_matrix = NULL;

    if (argc != 3) {
        return EXIT_FAILURE;
    }

    fptrain = fopen(argv[1], "r");
    fpdata = fopen(argv[2], "r");
    if (!fptrain || !fpdata) {
        return EXIT_FAILURE;
    }

    // data.txt matrix allocation

    fscanf(fpdata, "%4s%d%d", dstr, &dk, &dm);
    data_X_matrix = (double**)malloc(sizeof(double*)*dm);
    data_Y_matrix = (double**)malloc(sizeof(double*)*dm);
    for (int i = 0; i < dm; i++) {
        data_X_matrix[i] = (double*)malloc(sizeof(double)*(dk+1));
        data_Y_matrix[i] = (double*)malloc(sizeof(double)*1);
    }
    for (int i = 0; i < dm; i++) {
        data_X_matrix[i][0] = 1;
        for (int j = 1; j < dk+1; j++) {
            fscanf(fpdata, "%lf", &data_X_matrix[i][j]);
        }
    }

    // train.txt matrix allocation

    fscanf(fptrain, "%5s%d%d", tstr, &tk, &tn);
    if (dk != tk) {
        return EXIT_FAILURE;
    }
    train_X_matrix = (double**)malloc(sizeof(double*)*tn);
    train_Y_matrix = (double**)malloc(sizeof(double*)*tn);
    W_matrix = (double**)malloc(sizeof(double*)*(tk+1));
    for (int i = 0; i < tn; i++) {
        train_X_matrix[i] = (double*)malloc(sizeof(double)*(tk+1));
        train_Y_matrix[i] = (double*)malloc(sizeof(double*)*1);
    }
    for (int i = 0; i < tk+1; i++) {
        W_matrix[i] = (double*)malloc(sizeof(double)*1);
    }
    for (int i = 0; i < tn; i++) {
        train_X_matrix[i][0] = 1;
        for (int j = 1; j < tk+1; j++) {
            fscanf(fptrain, "%lf", &train_X_matrix[i][j]);
        }
        fscanf(fptrain, "%lf", &train_Y_matrix[i][0]);
    }

    train_XT_matrix = transpose_alloc(train_XT_matrix, tk, tn);
    train_XT_matrix = tranpose(train_X_matrix, train_XT_matrix, tk, tn);
    product = product_alloc(product, tk);
    inverse = product_alloc(inverse, tk);
    product = multiply_XT_X(product, train_X_matrix, train_XT_matrix, tn, tk);
    inverse = identity_creation(inverse, tk);
    /* for (int i = 0; i < tk+1; i++) {
        for (int j = 0; j < tk+1; j++) {
            printf("%f ", product[i][j]);
        }
        printf("\n");
    } */
    inverse = Gauss_Jordan(product, inverse, tk);
    psuedo = transpose_alloc(psuedo, tk, tn);
    psuedo = multiply_inverse_XT(inverse, train_XT_matrix, psuedo, tk, tn);
    W_matrix = multiply_psuedo_Y(psuedo, train_Y_matrix, W_matrix, tk, tn);

    free_tranpose(train_XT_matrix, tk);
    free_product(product, tk);
    free_product(inverse, tk);
    free_tranpose(psuedo, tk);
    free_train_X_matrix(train_X_matrix, tn);
    free_train_Y_matrix(train_Y_matrix, tn);

    multiply_XW(data_X_matrix, W_matrix, data_Y_matrix, dk, dm);
    for (int i = 0; i < dm; i++) {
        for (int j = 0; j < 1; j++) {
            printf("%.0f\n", data_Y_matrix[i][j]);
        }
    }
    free_data_X_matrix(data_X_matrix, dm);
    free_W_matrix(W_matrix, tk); 
    free_data_Y_matrix(data_Y_matrix, dm);



    return EXIT_SUCCESS;
}