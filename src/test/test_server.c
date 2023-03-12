#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../lib/linked_list.h"
#include "../lib/comm.h"
#include "test_server.h"


void testListSeq() {
    /*
    Test the linked list library sequentially
    */

    // create list
    List list;
    init(&list);
    printf("List created.\n");

    // insert elements
    char str[] = "pene";
    set(&list, 420, str, 69, 420.69);
    printList(list);
    printf("Elements inserted.\n");


    return;
}


void testListConc() {
    /*
    Test the linked list library concurrently
    */

    
    return;
}


int main() {

    testListSeq();

    return 0;
}