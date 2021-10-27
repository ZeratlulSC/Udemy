#include <stdio.h>

int main (void){
    int rows=0;
    int columns=0;
    printf("Enter number of rows for the matrix (>=3 and <=10): ");
    scanf("%d", &rows);
    printf("Enter number of columns for the matrix (>=3 and <=10): ");
    scanf("%d", &columns);
    if (rows > 10 || rows < 3) {
        printf("Invalid number of rows, acceptable is >=3 and <=10");
        return 0;
    }
    if (columns > 10 || columns < 3) {
        printf("Invalid number of columns, acceptable is >=3 and <=10");
        return 0;    
    }
    int matrix[rows][columns];
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            printf("Matrix[%d][%d]: ", i, j);
            scanf("%d", &matrix[i][j]);
        }
    }
    printf("Matrix is given by you: \n");
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    printf("Matrix is given by you: \n");
    for (int i = 0; i < columns ; i++){
        for (int j = 0; j < rows; j++){
            printf("%5d", matrix[j][i]);
        }
        printf("\n");
    }
	return 0;
}
