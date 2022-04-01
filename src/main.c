#include <stdio.h>
#include "app/matrix.c"

int main() {
    double array1[] = {1, 1, 1, 1,
                       1, 0, 1, 1,
                       1, 1, 1, 1};

    double array2[] = {1, 0, 1, 1,
                       0, 0, 10, 1,
                       0, 1, 1, 1};

    Matrix* matrix1 =  create_matrix(array1, 3, 4);
    Matrix* matrix2 =  create_matrix(array2, 3, 4);
    Matrix* matrix_add = add(matrix1, matrix2);
    Matrix* matrix_sub = subtract(matrix1, matrix2);  

    print_matrix(matrix_add);
    printf("\n");
    print_matrix(matrix_sub);

    printf("%f\n", *matrix_max(matrix2) );
    delete(matrix1);
    delete(matrix2);
    delete(matrix_add);
    delete(matrix_sub);

    return 0;
}
