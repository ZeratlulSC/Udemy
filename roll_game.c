#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
    int score, sum, roll1, roll2;
    int corr = 0, incorr = 0;
    long t;
    int guess = 0;
    while (1){
    t = time(NULL);
    srand(t);
    roll1 = rand() % 6 + 1;
    roll2 = rand() % 6 + 1;
    sum = roll1 + roll2;
    printf("Computer rolled two dice... Guess the sum\nEnter any <=0 value to terminate.\nEnter your guess: ");
    scanf("%d", &guess);
    if (guess < 1) return 0;
    if (guess == sum){
        score += 10;
        corr++;
        printf("Bravo!!! You were correct +10");
    }
    else{
        score -= 5;
        incorr++;
        printf("Darn!!! Bad attempt, -5\nSum was: %d", sum);
    }
    printf("\n\nTotal Correct: %d\nTotal Incorrect: %d\nScore: %d\n\n", corr, incorr, score);
    printf("dice 1 dice 2 %d %d\n", roll1, roll2);
    }
    return 0;
}
