#include <stdio.h>
#include <stdlib.h>


int main(){
    int *x = malloc(10 * sizeof(int));
    int y[10];
    printf("SIZE OF X[1] %ld\n", sizeof(x[1]));
    printf("SIZE OF Y %ld\n", sizeof(y));
    printf("SIZE OF *Y %ld\n", sizeof(*y));
    printf("SIZE OF X + 0 %ld\n", sizeof(x + 0));
    printf("SIZE OF X[0] %ld\n", sizeof(x[0]));
    *y = 8;
    printf("Y[0] %d\n", y[0]);
    return 0;
}