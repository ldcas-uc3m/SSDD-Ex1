/*
Implementación de las operaciones del cliente
*/

#include <stdio.h>
#include <mqueue.h>

#include "claves.h"
#include "comm.h"

#define INIT 0
#define SET_VALUE 1
#define GET_VALUE 2
#define MODIFY_VALUE 3
#define EXIST 4
#define COPY_KEY 5
#define SHUTDOWN 6

#define NUM_MENSAJES 10;


int init(void) {

    nameColaCliente = malloc(MAX_NAME_COLA * sizeof(char));

    char pid=malloc(MAX_NAME_COLA * sizeof(char));

    sprintf(pid,"%ld", (unsigned long int) pthread_self());

    strcat(nameColaCliente, pid);
    free(pid);

    struct Peticion nuevaPeticion;
    nuevaPeticion.opcode = INIT;
    nuevaPeticion.cola_client = nameColaCliente;
    //Meterlo en la cola

    mqd_t mqd;
    struct mq_attr atributos;
    atributos.mq_msgsize = sizeof(nuevaPeticion);
    atributos.mq_maxmsg = NUM_MENSAJES;    

    //Ver el codigo de error
    struct Respuesta res;
    if (res.result!=0){
        free(nameColaCliente);
        return -1;
    }

    return 0;
}


int set_value(int key, char* value1, int value2, double value3) {

    struct Tupla tuple;
    if (strlen(value1)>256){
        return -1;
    }

    tuple.clave = key;
    strcpy(tuple.value1, value1);
    tuple.value2 = value2;
    tuple.value3 = value3;

    struct Peticion nuevaPeticion;

    nuevaPeticion.opcode = SET_VALUE;
    nuevaPeticion.value = tuple;
    nuevaPeticion.alt_key = NULL;
    nuevaPeticion.cola_client = nameColaCliente;

    //Meterlo en la cola

    mqd_t mqd;
    struct mq_attr atributos;
    atributos.mq_msgsize = sizeof(nuevaPeticion);
    atributos.mq_maxmsg = NUM_MENSAJES;    

    //Ver el codigo de error
    struct Respuesta res;
    if (res.result!=0){
        free(nameColaCliente);
        return -1;
    }
    return 0;
}


int get_value(int key, char* value1, int* value2, double* value3) {

    struct Tupla tuple;
    tuple.clave = key;

    struct Peticion nuevaPeticion;

    nuevaPeticion.opcode = GET_VALUE;
    nuevaPeticion.value = tuple;
    nuevaPeticion.alt_key = NULL;
    nuevaPeticion.cola_client = nameColaCliente;

    mqd_t mqd;
    struct mq_attr atributos;
    atributos.mq_msgsize = sizeof(nuevaPeticion);
    atributos.mq_maxmsg = NUM_MENSAJES;    

    //Pedir la informacion a la cola
    struct Respuesta res;


    //Store informacion
    if (res.result!=0){
        free(nameColaCliente);
        return -1;
    }
    *value1 = res.value.value1;
    *value2 = res.value.value2;
    *value3 = res.value.value3;

    return 0;
}


int modify_value(int key, char* value1, int value2, double value3) {

    struct Tupla tuple;
    if (strlen(value1)>256){
        return -1;
    }

    tuple.clave = key;
    strcpy(tuple.value1, value1);
    tuple.value2 = value2;
    tuple.value3 = value3;

    struct Peticion nuevaPeticion;

    nuevaPeticion.opcode = MODIFY_VALUE;
    nuevaPeticion.value = tuple;
    nuevaPeticion.alt_key = NULL;
    nuevaPeticion.cola_client = nameColaCliente;

    mqd_t mqd;
    struct mq_attr atributos;
    atributos.mq_msgsize = sizeof(nuevaPeticion);
    atributos.mq_maxmsg = NUM_MENSAJES;    


    //Meterlo en la cola

    //Ver el codigo de error
    struct Respuesta res;
    if (res.result!=0){
        free(nameColaCliente);
        return -1;
    }
    return 0;
}


int exist(int key) {

    struct Tupla tuple;
    tuple.clave = key;

    struct Peticion nuevaPeticion;
    nuevaPeticion.opcode = EXIST;
    nuevaPeticion.value = tuple;
    nuevaPeticion.alt_key = NULL;
    nuevaPeticion.cola_client = nameColaCliente;

    //Meterlo en la cola

    mqd_t mqd;
    struct mq_attr atributos;
    atributos.mq_msgsize = sizeof(nuevaPeticion);
    atributos.mq_maxmsg = NUM_MENSAJES;    

    //Ver el codigo de error
    struct Respuesta res;
    if (res.result!=0){
        free(nameColaCliente);
        return -1;
    }

    return 0;
}


int copy_key(int key1, int key2) {

    struct Tupla tuple;
    tuple.clave = key1;

    struct Peticion nuevaPeticion;
    nuevaPeticion.opcode = COPY_KEY;
    nuevaPeticion.value = tuple;
    nuevaPeticion.alt_key = key2;
    nuevaPeticion.cola_client = nameColaCliente;

    //Meterlo en la cola

    mqd_t mqd;
    struct mq_attr atributos;
    atributos.mq_msgsize = sizeof(nuevaPeticion);
    atributos.mq_maxmsg = NUM_MENSAJES;    

    //Ver el codigo de error
    struct Respuesta res;
    if (res.result!=0){
        free(nameColaCliente);
        return -1;
    }
    return 0;
}

int shutdown(void){
    struct Tupla tuple;

    struct Peticion nuevaPeticion;
    nuevaPeticion.opcode = SHUTDOWN;

    mqd_t mqd;
    struct mq_attr atributos;
    atributos.mq_msgsize = sizeof(nuevaPeticion);
    atributos.mq_maxmsg = NUM_MENSAJES;    

    if (free(nameColaCliente)){
        return 0;
    }else{
        return -1;
    }
}
