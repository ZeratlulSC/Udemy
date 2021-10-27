#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
    float arr1[20], arr2[20], nmbr;
    int max_nmbr = 0;
    float sum1 = 0;
    float sum2 = 0;
    printf("Enter maximum 20 numbers, terminate with 0.0: \n");
    do
    {   
        if (max_nmbr == 0) printf("First number: ");
        else printf("Next: ");
        scanf("%f", &nmbr);
        if (nmbr != 0.0) arr1[max_nmbr++] = nmbr;
        else break;
    } while (max_nmbr < 20);
    printf("\nThere are in total %d numbers given as input\n\n", max_nmbr);
    printf("Input Array   2nd Array\n\n");
    for (int i = 0; i < max_nmbr;i++) {
        if (i%2 == 0) arr2[i] = arr1[i] * 2;
        else arr2[i] = cbrt(arr1[i]);
        printf("%11.5f %11.5f\n", arr1[i], arr2[i]);
        sum1 = sum1 + arr1[i];
        sum2 = sum2 + arr2[i];
    }
    printf("\n\nSum of ther first array: %f \nSum of the second array: %f", sum1, sum2);
    return 0;
}
