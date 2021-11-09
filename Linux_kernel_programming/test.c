#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main (int argc, char **argv){
    FILE *pfile = NULL;
    FILE *newfile = NULL;
    char *filename = "readme";
    char *filename1 = "new_file.txt";
    pfile = fopen(filename, "r");
    newfile = fopen(filename1, "w");
    if (pfile == NULL || newfile == NULL) return -1;
    char ch = fgetc(pfile);
    while (ch != EOF)
    {
        fputc(tolower(ch), newfile);
        ch = fgetc(pfile);
    }
    rename(filename1, filename);
    fclose(pfile);
    fclose(newfile);
    remove(filename1);
    pfile = NULL;
    newfile = NULL;
    return 0;
}