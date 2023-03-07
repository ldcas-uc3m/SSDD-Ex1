/*
Definición de las operaciones del servidor
*/

#ifndef _SERVER_IMPL_H_
#define _SERVER_IMPL_H_

#define DATA_PATH "../data/data"
#define BUF_MAX_SIZE 10


// sync file concurrency
pthread_mutex_t mutex_fd;


int init(void);
int set_value(int key, char* value1, int value2, double value3);
int get_value(int key, char* value1, int* value2, double* value3);
int modify_value(int key, char* value1, int value2, double value3);
int exist(int key);
int copy_key(int key1, int key2);


#endif