#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct employee 
{
    char *itemName;
    int quantity;
    float price;
    float amount;
};

void readItem (struct employee *emp){
    scanf("%s", emp->itemName);
    scanf("%f", &(emp->price));
    scanf("%d", &(emp->quantity));
    emp->amount = emp->quantity * emp->price;
}

void printItem (struct employee *emp){
    printf("%s %d %f %f", emp->itemName, emp->quantity, emp->price, emp->amount);
}

int main (int argc, char **argv){
    struct employee *emp1, emp;
    emp1 = &emp;
    emp1->itemName = (char *)malloc(50*sizeof(char));
    readItem(emp1);
    printItem(emp1);
    printf("1");
    free(emp.itemName);
    return 0;
}