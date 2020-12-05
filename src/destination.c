#include <netinet/in.h> // protocol macros
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void prepare_destination(struct sockaddr_in *sin) {
    memset(sin, 0, sizeof(*sin));
    sin->sin_family = AF_INET;

    char ip4[INET_ADDRSTRLEN];

    //convert human readable address form (with dots) to bit (network) form
    // check the return status, might be less than zero to indicate messed up address
    if (inet_pton(AF_INET, "192.168.1.1", &(sin->sin_addr)) < 0) {
        perror("inet_pton() error");
        exit(-1);
    }

    inet_ntop(AF_INET, &(sin->sin_addr), ip4, INET_ADDRSTRLEN);
    printf("address is %s\n\n", ip4);
}
