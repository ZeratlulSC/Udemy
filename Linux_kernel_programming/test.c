#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct employee 
{
    int hireDate;
    float salary;
    char name[40];
};


int main (int argc, char **argv){
    struct employee emp1;
    emp1.hireDate = 8112021;
    strcpy(emp1.name, "Jane");
    emp1.salary = 524.2;
    struct employee emp2;
    scanf("%d %f %s", &(emp2.hireDate), &(emp2.salary), emp2.name);
    printf("%s %f %d\n%s %f %d\n", emp1.name, emp1.salary, emp1.hireDate, emp2.name, emp2.salary, emp2.hireDate);
    return 0;
}