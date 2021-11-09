#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main (int argc, char **argv){
    FILE *file = NULL;
    file = fopen("readme", "r");
    fseek(file, 0, SEEK_END);
    int filesize = ftell(file);
    printf("%d\n", filesize);
    for (int i = 1; i <= filesize; i++){
        fseek(file, -i, SEEK_END);
        printf("%c", fgetc(file));
    }
    printf("\n");
    fclose(file);
    file = NULL;
    return 0;
}