#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // protocol macros
#include "ping.h"

int get_socket() {
    int sfd;

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
    return (sfd);
}

void send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin) {
    ssize_t bytes;

    if ((bytes = sendto(sfd, packet, sizeof(*packet), 0, (struct sockaddr *)sin, sizeof(*sin))) < 0)
    {
        perror("sendto() error");
        exit(-1);
    } else {
        printf("sent - %ld\n\n", bytes);
    }
}