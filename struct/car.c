// Please do not attempt to do anything other than what is instructed. This method will be called by the tester.
// You will not write any main method or any printf to print anything into the console for this assignment.

#include <stdio.h>
#include <string.h>
#include "car.h"

Car getCar(){
    // You will write statements to assign the following values for the Car attributes
    // Please look into the Car type in the header file.
    // Instruction 1: declare a Car object in the following line.
    Car car;
    // Instruction 2: Assign "Toyota" to the make property of the car object that you have declared. 
    // Since make is a character array, you need to use strcpy of string.h
    // Instruction 3: Assign "Camry" to the model property of the car object that you have declared.
    // since model is a character array, you will use strcpy for assigning the model .
    strcpy(car.make, "Toyota");
    strcpy(car.model, "Camry");
    // Instruction 4: Assign 2010 to the year property of the car object that you have declared.
    car.year = 2010;
    
    // Instruction 5: Return the car object that you have created to the caller using the return statement.
    //  Look into the return type of this function, it is Car, so you must return the car object that you have 
    // declared and initialized as per the instructions.
    
    return car;
    
     
}

int main (){
    getCar();
    Car *sp;
    return 0;
}