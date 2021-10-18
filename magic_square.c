#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
    int dim = 0;
    printf("Enter dimension of Magic Square (Only ODD dimension >=1 and <=15 accepted): ");
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
    for(int k = 1; k < dim*dim; k++){
        arr[row++][column++] = k;
        if (row == dim) row = 0;
        if (column == dim) dim = 0;
        if (arr[row][column] != 0){
        
        }
    }
    return 0;
}
