/*
Simple sequential test client
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../lib/claves.h"

#define MAX_VALUE1 256


int standard_run() {
    /*
    Tests all basic functions
    */

    // start list
    init();

    int key = 0;
    char value1[] = "Prueba";
    int value2 = 1;
    double value3 = 2.4;  

    // set
    if (set_value(key, value1, value2, value3) != 0) {
        perror("Error while inserting the value\n");
        return -1;
    }

    // check previous key exists
    if (exist(key) != 1) {
        perror("Key doesn't exist\n");
        return -1;
    }

    // copy key
    int newKey = 1;
    if (copy_key(key, newKey) != 0) {
        perror("Error while copying the key\n");
        return -1;
    }

    // modify value
    int newValue2 = 2;
    if (modify_value(newKey, value1, newValue2, value3) != 0) {
        perror("Error while modifying the tuple\n");
        return -1;
    }

    // get
    char* value1Get = malloc(MAX_VALUE1 * sizeof(char));
    int value2Get;
    double value3Get;

    if (get_value(newKey, &value1Get, &value2Get, &value3Get) == 0) {
        printf("The obtained values are %s , %d, %d\n", value1Get, value2Get, value3Get);
    }
    else {
        free(value1Get);
        perror("Error while getting the key\n");
        return -1;
    }
    
    // delete_key
    if (delete_key(newKey) != 0) {
        perror("Error while deleting the key\n");
        return -1;
    }

    shutdown();

    return 0;
}


int break_stuff() {
    /*
    Let's break some stuff!
    Just to see this works...
    */


   return 0;
}


int main() {
    if (standard_run() == -1) {
        perror("\n---Sequential List tests Failed---\n\n");
        return -1;
    }
}