/*
Implementación de las operaciones del cliente
*/

#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

#include "claves.h"
#include "comm.h"


#define INIT 0
#define SET_VALUE 1
#define GET_VALUE 2
#define MODIFY_VALUE 3
#define EXIST 4
#define COPY_KEY 5
#define DELETE_KEY 6
#define SHUTDOWN 7

#define NUM_MENSAJES 10;


char *nameColaCliente = NULL;


mqd_t abrirColaServer(){
    mqd_t qs = mq_open(SERVER_Q_NAME, O_WRONLY);  // server queue
    if (qs == -1) {
        perror("No se puede abrir la cola del servidor");
        return -1;
    }
    return qs;
}



mqd_t abrirColaCliente(){

    mqd_t qc;  // server queue
    struct mq_attr q_attr;

    q_attr.mq_maxmsg = MQUEUE_SIZE;
    q_attr.mq_msgsize = sizeof(struct Respuesta);

    // init queue
    qc = mq_open(nameColaCliente, O_CREAT|O_RDONLY, 0700, &q_attr);
    if (qc == -1) {
        perror("No se puede crear la cola del cliente\n");
        return -1;
    }

    return qc;
}

int cerrarCola(mqd_t queue_id){
    if (mq_close(queue_id) !=-1){
        mq_unlink(nameColaCliente);
        return 0;
    }else{
        return -1;
    }
}

int sendPeticion(struct Peticion *pet, struct Respuesta *res){
    //inicializar las queues
    mqd_t q_server = abrirColaServer();
    mqd_t q_cliente = abrirColaCliente();

    if (q_server == -1 || q_cliente == -1){
        return -1;
    }

    //Mandar la informacion
    if (mq_send(q_server, (const char*) pet, sizeof(struct Peticion), 0)==-1){
        cerrarCola(q_cliente);
        mq_close(q_server);
        return -1;
    }

    printf("%s: Sent {opcode: %i, key: %i, alt_key: %i, value1: %s, value2: %i, value3: %f}\n", pet->cola_client, pet->opcode, pet->value.clave, pet->alt_key, pet->value.value1, pet->value.value2, pet->value.value3);

    // Esperar a la respuesta
    if (mq_receive(q_cliente, (char*) res, sizeof(struct Respuesta), 0)==-1){
        cerrarCola(q_cliente);
        mq_close(q_server);
        return -1;
    }

    cerrarCola(q_cliente);
    mq_close(q_server);

    if (res->result == -1) {
        return -1;
    }

    return 0;
}


int init(void) {

    nameColaCliente = (char *) malloc(MAX_NAME_COLA * sizeof(char));
    sprintf(nameColaCliente, PREFIX);

    char* pid = (char *) malloc(MAX_NAME_COLA * sizeof(char));

    sprintf(pid,"%ld", (unsigned long int) pthread_self());

    strcat(nameColaCliente, pid);
    free(pid);

    printf("Nombre de cola: %s\n", nameColaCliente);

    struct Peticion pet;
    pet.opcode = INIT;
    pet.alt_key = 0;
    pet.value.clave = 0;
    strcpy(pet.cola_client, nameColaCliente);

    struct Respuesta res;

    int resultCode = sendPeticion(&pet, &res);

    if (resultCode==-1){
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

    struct Peticion pet;

    pet.opcode = SET_VALUE;
    pet.value = tuple;
    strcpy(pet.cola_client, nameColaCliente);

    struct Respuesta res;
    

    int resultCode = sendPeticion(&pet, &res);
    
    if (res.result==-1 || resultCode==-1){
        free(nameColaCliente);
        return -1;
    }
    return 0;
}


int get_value(int key, char* value1, int* value2, double* value3) {

    struct Tupla tuple;
    tuple.clave = key;

    struct Peticion pet;

    pet.opcode = GET_VALUE;
    pet.value = tuple;
    strcpy(pet.cola_client, nameColaCliente);

    struct Respuesta res;
        
    int resultCode = sendPeticion(&pet, &res);

    //Store informacion
    if (res.result==-1 || resultCode==-1){
        free(nameColaCliente);
        return -1;
    }

    strcpy(value1, tuple.value1);
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

    struct Peticion pet;

    pet.opcode = MODIFY_VALUE;
    pet.value = tuple;
    strcpy(pet.cola_client, nameColaCliente);
    
    struct Respuesta res;

    int resultCode = sendPeticion(&pet, &res);

    if (res.result==-1 || resultCode==-1){
        free(nameColaCliente);
        return -1;
    }
    return 0;
}


int exist(int key) {

    struct Tupla tuple;
    tuple.clave = key;

    struct Peticion pet;
    pet.opcode = EXIST;
    pet.value = tuple;
    strcpy(pet.cola_client, nameColaCliente);

    //Ver el codigo de error
    struct Respuesta res;

    int resultCode = sendPeticion(&pet, &res);

    if (res.result==-1 || resultCode==-1){
        free(nameColaCliente);
        return -1;
    }

    return res.result;
}


int copy_key(int key1, int key2) {

    struct Tupla tuple;
    tuple.clave = key1;

    struct Peticion pet;
    pet.opcode = COPY_KEY;
    pet.value = tuple;
    pet.alt_key = key2;
    strcpy(pet.cola_client, nameColaCliente);

    //Ver el codigo de error
    struct Respuesta res;

    int resultCode = sendPeticion(&pet, &res);

    if (res.result==-1 || resultCode==-1){
        free(nameColaCliente);
        return -1;
    }
    return 0;
}


int delete_key(int key){

    struct Tupla tuple;
    tuple.clave = key;

    struct Peticion pet;
    pet.opcode = DELETE_KEY;
    pet.value = tuple;
    strcpy(pet.cola_client, nameColaCliente);

    struct Respuesta res;

    int resultCode = sendPeticion(&pet, &res);

    if (res.result==-1 || resultCode==-1){
        free(nameColaCliente);
        return -1;
    }
    return 0;
}


int shutdown(void){

    struct Peticion pet;
    pet.opcode = SHUTDOWN;

    struct Respuesta res;

    int resultCode = sendPeticion(&pet, &res);
    free(nameColaCliente);

    return resultCode;
}