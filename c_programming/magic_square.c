#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int dim = 0;
    printf("Enter dimension of Magic Square (Only ODD dimension >=1 and <=15 accepted):");
    scanf("%d", &dim);
    if (dim%2==0 || dim < 1 || dim > 15){
        printf("Invalid input\nValid values are odd integers between 1 and 15");
        return 0;
    }
    int arr [dim][dim];
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++) arr[i][j] = 0;
    }
    int row = dim - 1;
    int column = dim/2;
    int save_r = 0;
    int save_c = 0;
    for(int k = 1; k <= dim*dim; k++){
        save_r = row;
        save_c = column;
        arr[row++][column++] = k;
        if (row == dim){
            row = 0;
        }
        if (column == dim) {
            column = 0;    
        }
        if (arr[row][column] != 0){
            row = save_r;
            column = save_c;
            row--;
        }
    }
    int magic_const = dim*(dim*dim + 1)/2;
    for (int i = 0; i < dim; i++){
        int sum = 0;
        for (int j = 0; j < dim; j++){
            sum = sum + arr[i][j];
        }
        if (sum != magic_const) {
            printf("Error in forming magic square with dimension: %d", dim);
            return 1;
        }
    }
    for (int i = 0; i < dim; i++){
        int sum = 0;
        for (int j = 0; j < dim; j++){
            sum = sum + arr[j][i];
        }
        if (sum != magic_const){
            printf("Error in forming magic square with dimension: %d", dim);
            return 1;
        }    
    }
    int sum = 0;
    for (int i = 0; i < dim; i++){
        sum = sum + arr[i][i];
    }
    if (sum != magic_const){
        printf("Error in forming magic square with dimension: %d", dim);
        return 1;
    }
    sum = 0;
    for (int i = 0; i < dim; i++){
        sum = sum + arr[i][dim - 1 - i];
    }
    if (sum != magic_const){
        printf("Error in forming magic square with dimension: %d", dim);
        return 1;
    }
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++) printf("%3d ", arr[i][j]);
        printf("\n");
    }
    printf("Magic square generated with dimension %d and magic constant: %d", dim, magic_const);
    return 0;
}
