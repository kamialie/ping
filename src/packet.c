#include "ping.h"
#include <string.h>
#include <arpa/inet.h>

// icmp packet types
#include <netinet/ip_icmp.h>

void fill_icmp_packet(t_icmp_pack *packet) {
    memset(packet, 0, sizeof(*packet));

    packet->header.type = ICMP_ECHO;
    packet->header.code = 0;
    packet->header.id = htons(0);
    packet->header.seq = htons(0);
    //packet->data.content = 0;
    packet->header.chksum = 0;
    memcpy(&(packet->data), "hi!", strlen("hi!"));

    packet->header.chksum = htons(compute_checksum((u_int16_t *)packet, sizeof(*packet)));//htons(0x6e87);//htons(~(ICMP_ECHO << 8));// htons(checksum((uint16_t *)packet, sizeof(*packet)));

    printf("size of packet - %ld\n", sizeof(*packet));
    printf("icmp message type - %d\n", packet->header.type);
    printf("icmp message seq - %d\n", packet->header.seq);
    printf("icmp message chksum - %d\n", packet->header.chksum);
    print_memory(packet, sizeof(*packet));
}

void receive_packet(int sfd) {
    struct msghdr msg;
    struct iovec io;
    struct sockaddr_in rec_addr;
    char buf[512];
    ssize_t b;

    memset(buf, 0, sizeof(buf));
    memset(&msg, 0, sizeof(msg));
    memset(&rec_addr, 0, sizeof(rec_addr));

    io.iov_base = buf;
    io.iov_len = sizeof(buf);

    msg.msg_name = &rec_addr;
    msg.msg_namelen = sizeof(rec_addr);
    msg.msg_control = buf + 256;
    msg.msg_controllen = 256;
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    if ((b = recvmsg(sfd, &msg, 0)) < 0 )
    {
        perror("revmsg() error");
        exit(-1);
    } else {
        printf("received - %ld\n", b);
    }

    t_icmp_pack *icmp_in;
    icmp_in = (t_icmp_pack *)(20 + buf);

    printf("size of message - %ld\n", msg.msg_iov[0].iov_len);
    printf("incoming address - %d\n", ntohl(*((int*)&rec_addr.sin_addr)));
    printf("icmp message type - %d\n", icmp_in->header.type);
    printf("icmp message seq - %d\n", ntohs(icmp_in->header.seq));
    printf("icmp message id - %d\n", ntohs(icmp_in->header.id));
    print_memory(icmp_in, sizeof(*icmp_in));
}