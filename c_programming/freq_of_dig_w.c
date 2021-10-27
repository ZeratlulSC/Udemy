#include <stdio.h>
#include <string.h>

void count_digits(char str[])
{
    // Find the number of each digits in the character array str supplied. 
    // You can safely asssume that there is a NULL at the end of the string str.
    // You will need to print the frequency of each digit as described in the problem statement.
    int arr[10]={0};
    for (int i = 0; i < strlen(str);i++){
        if (str[i]>47 && str[i] < 58) arr[str[i]-'0']++;
    }
    for (int i = 0; i<10;i++) printf("%d ", arr[i]);
}

int main (void){
    char str[] = "11122233344asdfsdf2314234499999999994wersdfa4555";
    count_digits(str);
    return 0;
}