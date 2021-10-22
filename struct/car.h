// Please do not change anything in this header.
#ifndef EXERCISE_H
#define EXERCISE_H
// The following is the structure type Car
typedef struct {
    char make[20];    // company that makes the car
    char model[20];   // model of the car
    int year;         // year of making
}Car;

Car getCar();

#endif // _EXERCISE_H
