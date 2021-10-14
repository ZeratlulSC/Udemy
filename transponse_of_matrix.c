#include <stdio.h>

int main (void){
    int rows=0;
    int columns=0;
    printf("Enter number of rows for the matrix (>=3 and <=10): ");
    scanf("%d", &rows);
    printf("Enter number of columns for the matrix (>=3 and <=10): ");
    scanf("%d", &columns);
    int matrix[rows][columns];
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            printf("Matrix[%d][%d]: ", i, j);
            scanf("%d", &matrix[i][j]);
        }
    }
	return 0;
}
