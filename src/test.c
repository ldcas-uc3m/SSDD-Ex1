

#include "tests/test_server.h"


int main() {
    // test linked list
    if (testListSeq() == -1) {
        perror("\n---Sequential List tests Failed---\n\n");
        return -1;
    }

    if (testListSeq() == -1) {
        perror("\n---Sequential List tests Failed---\n\n");
        return -1;
    }

    return 0;
}