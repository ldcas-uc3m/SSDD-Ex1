/*
Example client file
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib/claves.h"


#define MAX_VALUE1 256


int main() {

    // initialize the service
    init();


    // do some work, e.g. set some value
    int key = 0;
    char* value1 = "caca";
    int value2 = 420;
    double value3 = 6.9;  

    if (set_value(key, value1, value2, value3) != 0) {
        printf("Error while inserting the value");
        return -1;
    }

    // let's check it's in there...

    char* value1Get = malloc(MAX_VALUE1 * sizeof(char));
    int value2Get;
    double value3Get;

    if (get_value(key, value1Get, &value2Get, &value3Get) == 0) {
        printf("The obtained values are %s , %d, %f\n", value1Get, value2Get, value3Get);
    } else {
        free(value1Get);
        return -1;
    }


    // end with a shutdown
    shutdown();

    return 0;
}