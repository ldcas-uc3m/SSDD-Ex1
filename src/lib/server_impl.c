/*
Implementación de las operaciones del servidor
*/

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "server_impl.h"
#include "comm.h"


/* Values will be saved separated by ';', each one in a line */


/* All functions return 0 if no error and -1 if error */


int init(void) {
    /*
    Creates the file to save the data. If the file already exists, it deletes it.
    */

    // init mutex
	pthread_mutex_init(&mutex_fd, NULL);

    // create file
    FILE* fd;
    fd = fopen(DATA_PATH, "w");

    if (fd == NULL) {
        fprintf(stderr, "Error al crear el archivo %s", DATA_PATH);
        fclose(fd);
        return -1;
    }

    fclose(fd);

    return 0;
}


int set_value(int key, char* value1, int value2, double value3) {
    /*
    Inserts the tuple. Doesn't allow duplicate keys.
    */

    int curr_key;

    FILE* fd;
    fd = fopen(DATA_PATH, "a+");
    if (fd == NULL) {
        fprintf(stderr, "Error al acceder al archivo %s", DATA_PATH);
        fclose(fd);
        return -1;
    }

    // see if there is already a key
    while (fscanf(fd, "%n", &curr_key) != EOF) {
        if (fd == NULL) {
            fprintf(stderr, "Error al leer el archivo %s", DATA_PATH);
            fclose(fd);
            return -1;
        }

        if (curr_key == key) {
            fclose(fd);
            return -1;
        }
    }

    // write key
    pthread_mutex_lock(&mutex_fd);
    fprintf(fd, "%i;%s;%i;%d;", value1, value2, value3);
    pthread_mutex_unlock(&mutex_fd);

    if (fd == NULL) {
        fprintf(stderr, "Error al escribir al archivo %s", DATA_PATH);
        fclose(fd);
        return -1;
    }

    fclose(fd);
    return 0;
}


int get_value(int key, char* value1, int* value2, double* value3) {
    FILE* fd;
    fd = fopen(DATA_PATH, "r");


    fclose(fd);
    return 0;
}


int modify_value(int key, char* value1, int value2, double value3) {
    FILE* fd;
    fd = fopen(DATA_PATH, "r+");


    fclose(fd);
    return 0;
}


int exist(int key) {
    FILE* fd;
    fd = fopen(DATA_PATH, "r");


    fclose(fd);
    return 0;
}


int copy_key(int key1, int key2) {
    FILE* fd;
    fd = fopen(DATA_PATH, "a+");


    fclose(fd);
    return 0;
}