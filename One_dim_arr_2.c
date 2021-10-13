#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
    int nmbr;
    int arr[10] = {0};
    printf("Please note that you can input at most 10 digits number\nWhat is the total number of digits in the number? ");
    scanf("%d", &nmbr);
    if (nmbr>10) {
        printf("\nTotal number more than 10");
        return 0;
    }
    for (int i = 0; i < nmbr; i++) {
        printf("Input next digit: ");
        scanf("%d", &arr[i]);
        printf("\n");
    }
    printf("\nThe number that you gave is: ");
    for (int i = 0; i < nmbr; i++){
        printf("%d", arr[i]);
    }

    return 0;
}
