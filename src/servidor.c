#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "lib/linked_list.h"
#include "lib/comm.h"


// sync petitions
pthread_mutex_t mutex_pet;
pthread_cond_t c_pet;  // variable condicional de bloqueo
bool copiado = false;  // variable condicional de control


// list
List list;
bool isInit = false;


void tratar_peticion(struct Peticion* p) {
    struct Peticion pet;
    
    // copy petition
	pthread_mutex_lock(&mutex_pet);

    pet = *p;

    copiado = true;  // update conditional variable
	pthread_cond_signal(&c_pet);  // awake main
	pthread_mutex_unlock(&mutex_pet);


    // treat petition
    struct Respuesta res;

    switch (pet.opcode) {
        case 0:
            if (isInit) {
                res.result = -1;
                break;
            }
            res.result = init(&list);
            isInit = true;
            break;

        case 1:
            res.result = set(&list, pet.value.clave, pet.value.value1, pet.value.value2, pet.value.value3);
            break;

        case 2:
            res.result = get(&list, pet.value.clave, res.value.value1, &(res.value.value2), &(res.value.value3));
            break;
        
        case 3:
            res.result = modify(&list, pet.value.clave, pet.value.value1, pet.value.value2, pet.value.value3);
            break;
        
        case 4:
            res.result = exist(&list, pet.value.clave);
            break;

        case 5:
            res.result = copyKey(&list, pet.value.clave, pet.alt_key);
            break;
        
        default:
            res.result = -1;
            break;
    }

    // answer
    mqd_t qc = mq_open(pet.cola_client, O_WRONLY);  // client queue
    if (qc == -1) {
        perror("No se puede abrir la cola del cliente");
        pthread_exit(NULL);
    }
    mq_send(qc, (const char*) &res, sizeof(res), 0);
    mq_close(qc);

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    // queue stuff
    mqd_t qs;  // server queue
    struct mq_attr q_attr;
    struct Peticion msg;  // message to receive

    q_attr.mq_maxmsg = MQUEUE_SIZE;
    q_attr.mq_msgsize = sizeof(struct Peticion);

    // thread stuff
    pthread_attr_t t_attr;
    pthread_t thid;


    // init queue
    qs = mq_open(SERVER_Q_NAME, O_CREAT|O_RDONLY, 0700, &q_attr);
    if (qs == -1) {
        perror("No se puede crear la cola del servidor\n");
        return -1;
    }


    // init mutex and cond
	pthread_cond_init(&c_pet, NULL);
	pthread_mutex_init(&mutex_pet, NULL);

	pthread_attr_init(&t_attr);
    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);


    /* MAIN LOOP */
    while (true) {  // TODO: exit condition for loop
        // receive message
        mq_receive(qs, (char*) &msg, sizeof(struct Peticion), 0);

        // create thread
        if (pthread_create(&thid, &t_attr, (void*) tratar_peticion, (void*) &msg) == 0) {  // wait to copy petition
            // mutex logic
            pthread_mutex_lock(&mutex_pet);

            while (!copiado) {  // wait for thread to copy petition
                pthread_cond_wait(&c_pet, &mutex_pet);
            }

            copiado = false;  // reset variable
            pthread_mutex_unlock(&mutex_pet);

        } else {
            perror("Error al crear el thread\n");
            return -1;
        }
    }


    // cleanup
    destroy(&list);

    mq_close(qs);

	pthread_cond_destroy(&c_pet);
	pthread_mutex_destroy(&mutex_pet);

	exit(0);
}