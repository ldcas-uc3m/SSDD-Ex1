#include <stdio.h>
#include <mqueue.h>

#include "lib/server_impl.h"
#include "lib/comm.h"


void tratar_peticion(struct Peticion* p) {
    
    // copy petition
    struct Peticion pet;
    pet = *p;

    struct Respuesta res;

    // despertar main 
    
}


int main() {
    return 0;
}