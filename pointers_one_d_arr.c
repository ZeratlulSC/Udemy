#include <iostream>

using namespace std;

// declare the prototype of get_max
int get_max (int* p_int_arr, int n);
// declare the prototype of get_min
int get_min (int* p_int_arr, int n);

// Now implement the function get_max
int get_max (int* p_int_arr, int n){
    int ret = *(p_int_arr);
    for (int i = 0; i < n; i++){
        if (*(p_int_arr+i) > ret) ret = *(p_int_arr+i);
    }
    return ret;
}
// Now implement the function get_min

int get_min (int* p_int_arr, int n){
    int ret = *(p_int_arr);
    for (int i = 0; i < n; i++){
        if (*(p_int_arr+i) < ret) ret = *(p_int_arr+i);
    }
    return ret;
}

void get_max_min_sum(int *arr, int n, int *p_sum){
    
    // call get_max to get the maximum in the array arr, you will need to pass the base address arr and n to this function.
    *(p_sum) = get_max(arr, n) + get_min(arr, n);
    //ee
    // Call get_min in the similar way and get the minimum value in the array, here also you will need to pass 2 parameters, base address of the array and the number of elements in that array.
    
    
    // find the sum of the maximum and minimum and then assign the sum to the integer pointed by p_sum.
    
}