#include <stdio.h>
#include <mqueue.h>
#include <pthread.h>

#include "lib/server_impl.h"
#include "lib/comm.h"


void tratar_peticion(struct Peticion* p) {
    
    // copy petition
    struct Peticion pet;
    pet = *p;

    // awake main

    // treat petition
    struct Respuesta res;

    switch (pet.opcode) {
        case 0:
            res.result = init();
            break;

        case 1:
            res.result = set_value(pet.value.clave, pet.value.value1, pet.value.value2, pet.value.value3);
            break;

        case 2:
            res.result = get_value(pet.value.clave, &(res.value.value1), &(res.value.value2), &(res.value.value3));
            break;
        
        case 3:
            res.result = modify_value(pet.value.clave, pet.value.value1, pet.value.value2, pet.value.value3);
            break;
        
        case 4:
            res.result = exist(pet.value.clave);
            break;

        case 5:
            res.result = copy_key(pet.value.clave, pet.alt_key);
            break;
        
        default:
            res.result = -1;
            break;
    }

    // answer
    mqd_t qc = mq_open(pet.cola_client, O_WRONLY);  // client queue
    mq_send(qc, &res, sizeof(res), 0);
    mq_close(qc);

    pthread_exit(NULL);
}


int main() {
    

    return 0;
}