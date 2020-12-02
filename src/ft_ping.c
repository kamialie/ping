#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // protocol macros

#include <arpa/inet.h>

#include <netdb.h>

#include <stdlib.h>
#include <memory.h>

#include "ping.h"

int	main()
{
    int sfd, value;
    struct sockaddr_in sin;
    char ip4[INET_ADDRSTRLEN];
    char buf[400];

    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    {
        perror("socket() error");
        exit(1);
    }

    // tell kernel we gonna include IP header ourselves
    value = 1;
    if(setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &value, sizeof(value)) < 0)
    {
        perror("setsockopt() error");
        exit(-1);
    }
    else
        printf("setsockopt() is OK\n");

    // prepare info about destination host, that is address family and address itself
    sin.sin_family = AF_INET;

    //convert human readable address form (with dots) to bit (network) form
    // check the return status, might be less than zero to indicate messed up address
    if (inet_pton(AF_INET, "127.0.0.1", &(sin.sin_addr)) < 0) {
        perror("inet_pton() error");
        exit(-1);
    }
    inet_ntop(AF_INET, &(sin.sin_addr), ip4, INET_ADDRSTRLEN);
    printf("address is %s\n", ip4);

    ssize_t b;
    if ((b = sendto(sfd, buf, sizeof(buf), 0, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
        perror("sendto() error");
        exit(-1);
    } else {
        printf("sent - %ld\n", b);
    }

    return (0);
}
