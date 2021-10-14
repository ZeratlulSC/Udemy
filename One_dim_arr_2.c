#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
    int nmbr;
    int arr[10] = {0};
    int nmbr_to_add = 0;
    printf("Please note that you can input at most 10 digits number\nWhat is the total number of digits in the number? ");
    scanf("%d", &nmbr);
    if (nmbr>10) {
        printf("\nTotal number more than 10");
        return 0;
    }
    for (int i = nmbr; i > 0; i--) {
        printf("Input next digit: ");
        scanf("%d", &arr[nmbr - i]);
    }
    printf("The number that you gave is: ");
    for (int i = 0; i < nmbr; i++){
        printf("%d", arr[i]);
    }
    printf("\nEnter a digit >=1 and <=9 to add: ");
    scanf("%d", &nmbr_to_add);
    int perenos = 0;
    int safe = 0;
    for(int i = nmbr; i > 0 ; i--){
        safe = arr[i - 1];
        arr[i-1]= (arr[i-1] + nmbr_to_add*(i==nmbr) + perenos)%10;
        perenos = (safe + nmbr_to_add*(i==nmbr) + perenos)/10;
    }
    printf("The number that you gave is: \n");
    if (perenos && nmbr != 10) printf("1");
    for (int i = 0; i < nmbr; i++){
        printf("%d", arr[i]);
    }
    return 0;
}
