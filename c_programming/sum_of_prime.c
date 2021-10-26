#include <math.h>

int isPrime(int k){
    int i;
    // need to find factor in the range 2 to sqrt(k), if any factor could be found 
    // then the number is not a prime, otherwise, if no factor is there in the entire 
    // range, then it is a prime number.
    for(i = 2; i <= sqrt(k); ++i){
        if (k % i == 0){
            return 0;
        }
    }
    // no factor in the range, so it is a prime number.
    return 1;
}
//---------------- Please do not change anything in the above function.
// --------------------------------------- 
// Write your function just below, please note, the name of the function should be sum_prime
//-----------------------------------------
int sum_prime (int n){
    int sum = 0;
    for (int i = 2; i <= n; i++){
        if (isPrime(i)) sum = sum + i;
        printf("%d\n", sum);
    }
    return sum;
}

int main (void){
    printf("%d", sum_prime(10));
    return 0;
}