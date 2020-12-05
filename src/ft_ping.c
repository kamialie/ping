#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // protocol macros

#include <arpa/inet.h>

#include <netdb.h>

#include <memory.h>

#include "ping.h"

uint16_t checksum (uint16_t *addr, int len);
int	main()
{
    int sfd;
//    char on;
    struct sockaddr_in sin;
    char ip4[INET_ADDRSTRLEN];
//    char buf[400];
    t_icmp_pack packet;

    fill_icmp_packet(&packet);

    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        perror("socket() error");
        exit(1);
    }

    // tell kernel we gonna include IP header ourselves
//    on = 1;
//    if (setsockopt(sfd, IPPROTO_UDP, IP_HDRINCL, (int[1]){1},
//                   sizeof(int32_t)) != -1)
//    {
//        perror("setsockopt() error");
//        exit(-1);
//    }
//    if(setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
//    {
//        perror("setsockopt() error");
//        exit(-1);
//    }
//    else
//        printf("setsockopt() is OK\n");

    // prepare info about destination host, that is address family and address itself
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    //convert human readable address form (with dots) to bit (network) form
    // check the return status, might be less than zero to indicate messed up address
    if (inet_pton(AF_INET, "192.168.1.1", &(sin.sin_addr)) < 0) {
        perror("inet_pton() error");
        exit(-1);
    }
    inet_ntop(AF_INET, &(sin.sin_addr), ip4, INET_ADDRSTRLEN);
    printf("address is %s\n\n", ip4);

    ssize_t b;
    if ((b = sendto(sfd, &packet, sizeof(packet), 0, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
        perror("sendto() error");
        exit(-1);
    } else {
        printf("sent - %ld\n\n", b);
    }

    receive_packet(sfd);

    return (0);
}

uint16_t checksum (uint16_t *addr, int len)
{
    int count = len;
    register uint32_t sum = 0;
    uint16_t answer = 0;

    // Sum up 2-byte values until none or only one byte left.
    while (count > 1) {
        sum += *(addr++);
        count -= 2;
    }

    // Add left-over byte, if any.
    if (count > 0) {
        sum += *(uint8_t *) addr;
    }

    // Fold 32-bit sum into 16 bits; we lose information by doing this,
    // increasing the chances of a collision.
    // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    // Checksum is one's compliment of sum.
    answer = ~sum;

    return (answer);
}