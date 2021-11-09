#include <stdio.h>
#include <string.h>


int main (int argc, char **argv){
    int nmbr_of_lines = 1;
    FILE *pfile = NULL;
    char *filename = "../.gitignore";
    pfile = fopen(filename, "r");
    if (pfile == NULL) return -1;
    char ch = fgetc(pfile);
    while (ch!= EOF)
    {
        ch = fgetc(pfile);
        if (ch == '\n') nmbr_of_lines++;
    }
    fclose(pfile);
    pfile = NULL;
    printf("%d\n", nmbr_of_lines);
    return 0;
}