#include <stdio.h>

int main (){
    FILE *fp;
    
    fp = fopen("1.txt", "r");
    if (fp == NULL) {
        printf ("Unable to open");
        exit(1);
    }
    char ch = fgetc(fp);
    while(ch != EOF) {
        printf("%c", ch);
        ch = fgetc(fp);
    }
    printf("\n");
    fclose(fp);

    char str[30];
    fp = fopen("1.txt", "r");
    printf("%s", fgets(str, 30, fp));
    fclose(fp);
}