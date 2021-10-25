// You will not change anything in this file.

#ifndef EXERCISE_H
#define EXERCISE_H



typedef struct {
    char title[20];    // title of book.
    char author[20];   // author of book
    double price;      // price of book.
}Book;


double getCourierCharge(Book *b_ptr, unsigned distance);

#endif // _EXERCISE_H
