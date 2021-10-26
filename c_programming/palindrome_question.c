#include <stdio.h>
#include <string.h>

/*
A palindrome is a string that is same in both forward and backward reading.
Example:
   "madam"
   "racecar"
   "a man a plan a canal panama"
   "radar"
You will write a program that will test if a given string is a palingdrome or not.
Your program will ask the user to input a string and if the string is a palindrome program
will just print "Yes, it is a Palindrome", otherwise will print "No, not a Palindrome".

Please note that:
1. Your you need to check in case-insensitive way, that means: Madam or madam both should be
detected as Palindrome.

2. There can be (any number of ) spaces in between the words.
    "A man a plan a canal panama"
        OR
    "A     man    a   pla n a cana l Panama"
    both the strings must be detected as Palindrome.
3.There can be punctuations in between the words, for this assignments,
we consider only 4 punctuations,   . ?  ! and ,

Your program will just need to ignore them (treat them as space).
    "Cigar? Toss it in a can. It is so tragic."
    Should be detected as palindrome.

 *** For this assignment I will not write any instructions or guidance, you are free
        to implement it with your own way, you can use the string.h functions

    Good luck.

*/


int main(){
    char string [100];
    char string2 [100];
    printf("Enter a string: ");
    scanf("%[^\n]", string);

    int isPalindrome = 1;  // assign 0 to this if the string is a NOT palindrome

    // write code to test if string is a palindrome
    int j = 0;
    for (int i = 0; i < strlen(string); i++){
        if (string[i] > 96 && string[i] < 123) string2[j++] = string[i];
        else if (string[i] > 64 && string[i] < 91) string2[j++]=string[i] + ' ';
    }
    for (int i = 0; i < strlen(string2)/2; i++){
        if (string2[i] != string2[strlen(string2) - 1 - i]){
            isPalindrome = 0;
            break;
        }
    }
    // at the end you need to test
    if (isPalindrome){
        printf("Yes, it is Palindrome!\n");
    }
    else{
        printf("No, not a Palindrome\n");
    }


    return 0;
}
