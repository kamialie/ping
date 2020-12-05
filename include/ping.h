#ifndef PING_H
# define PING_H

// TEMPRORARY
#include <stdio.h>
//END
#include <stdlib.h> // u_int8_t
#include <netinet/in.h> // protocol macros


typedef struct s_icmp_hdr {
    u_int8_t    type; // message type, 0 - echo reply, 8 - echo request
    u_int8_t    code; // used for error code
    u_int16_t   chksum; // checksum of header plus data
    u_int16_t   id; // identifier used to match requests/replies
    u_int16_t   seq; // used to identify the sequence if more than one is sent
}   t_icmp_hdr;

typedef struct s_icmp_data {
    u_int32_t   content;
    u_int32_t   other;
    u_int32_t   third;
}   t_icmp_data;

// 96 bit, 12 bytes
typedef struct s_icmp_pack {
    t_icmp_hdr  header;
    t_icmp_data data;
}   t_icmp_pack;

typedef struct s_info {
    int sfd;
    t_icmp_pack icmp_packet;
    struct sockaddr_in  sin;
}   t_info;

int     get_socket();
void    fill_icmp_packet(t_icmp_pack *packet);
u_int16_t compute_checksum(u_int16_t *addr, int count);
void    prepare_destination(struct sockaddr_in *sin);
void    send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin);
void    receive_packet(int sfd);

void    print_memory(void *memory, unsigned int len);

#endif
