#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // protocol macros

#include <string.h>
#include <sys/time.h>

#include "ping.h"

int get_socket() {
    int sfd;

    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        perror("socket() error");
        exit(1);
    }
    // TODO add setsockopt to change ttl
    return (sfd);
}

void send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin) {
    ssize_t bytes;

    if ((bytes = sendto(sfd, packet, sizeof(*packet), 0, (struct sockaddr *)sin, sizeof(*sin))) < 0)
    {
        perror("sendto() error");
        exit(-1);
    }
//    else {
//        printf("sent - %ld\n\n", bytes);
//    }
    g_rt_stats.pkg_sent++;
}

//void receive_packet(int sfd) {
//    struct msghdr msg;
//    struct iovec io;
//    struct sockaddr_in rec_addr;
//    char buf[512];
//    ssize_t bytes;
//
//    memset(buf, 0, sizeof(buf));
//    memset(&msg, 0, sizeof(msg));
//    memset(&rec_addr, 0, sizeof(rec_addr));
//
//    io.iov_base = buf;
//    io.iov_len = sizeof(buf);
//
//    msg.msg_name = &rec_addr;
//    msg.msg_namelen = sizeof(rec_addr);
//    msg.msg_control = buf + 256;
//    msg.msg_controllen = 256;
//    msg.msg_iov = &io;
//    msg.msg_iovlen = 1;
//
//    if ((bytes = recvmsg(sfd, &msg, 0)) < 0 )
//    {
//        perror("revmsg() error");
//        exit(-1);
//    } else {
//        printf("received - %ld\n", bytes);
//    }
//}
