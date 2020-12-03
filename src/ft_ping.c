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
    int sfd;
    char on;
    struct sockaddr_in sin;
    char ip4[INET_ADDRSTRLEN];
    char buf[400];
    t_icmp_pack *packet;

    packet = (t_icmp_pack *)buf;
    memset(packet, 0, sizeof(*packet));
    packet->header.type = 8;
    packet->header.id = 1;
    packet->header.seq = 1;
    packet->data.content = 15;
    //memcpy(&(packet->data), "hello", strlen("hello"));
    printf("size of packet - %ld\n", sizeof(packet));

    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        perror("socket() error");
        exit(1);
    }

    // tell kernel we gonna include IP header ourselves
    on = 1;
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
    if (inet_pton(AF_INET, "127.0.0.1", &(sin.sin_addr)) < 0) {
        perror("inet_pton() error");
        exit(-1);
    }
    inet_ntop(AF_INET, &(sin.sin_addr), ip4, INET_ADDRSTRLEN);
    printf("address is %s\n", ip4);

    ssize_t b;
    if ((b = sendto(sfd, buf, sizeof(*packet), 0, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
        perror("sendto() error");
        exit(-1);
    } else {
        printf("sent - %ld\n", b);
    }
    struct msghdr msg;
    struct iovec io;
    char content[400];
    struct sockaddr_in rec_addr;
    char control[400];
    memset(content, 0, 400);
    memset(&rec_addr, 0, sizeof(rec_addr));
    memset(control, 0, 400);
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = &rec_addr;
    msg.msg_namelen = sizeof(rec_addr);
    msg.msg_control = control;
    msg.msg_controllen = 400;
    io.iov_base = content;
    io.iov_len = 400;
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    printf("size of sockaddr_in - %ld\n", sizeof(rec_addr));

    if ((b = recvmsg(sfd, &msg, 0)) < 0 )
    {
        perror("revmsg() error");
        exit(-1);
    } else {
        printf("received - %ld\n", b);
    }

    printf("message - %ud\n", *(u_int32_t *)&(io.iov_base));
    printf("incoming address - %d\n", *((int*)&rec_addr.sin_addr));

    return (0);
}
