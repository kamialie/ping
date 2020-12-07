#ifndef PING_H
# define PING_H

// TEMPRORARY
#include <stdio.h>
//END
#include <stdlib.h> // u_int8_t
#include <netinet/in.h> // protocol macros

#define DEFAULT_TTL 37
#define DEFAULT_ICMP_DATA 56
#define DEFAULT_TIMEOUT 1 // in seconds

// 64 bit, 8 bytes
typedef struct s_icmp_hdr {
    u_int8_t    type; // message type, 0 - echo reply, 8 - echo request
    u_int8_t    code; // used for error code
    u_int16_t   chksum; // checksum of header plus data
    u_int16_t   id; // identifier used to match requests/replies
    u_int16_t   seq; // used to identify the sequence if more than one is sent
}   t_icmp_hdr;

// 96 bit, 24 bytes
typedef struct s_icmp_pack {
    t_icmp_hdr  header;
    struct timeval  tv;
}   t_icmp_pack;

typedef struct s_ip_pack {
    unsigned char         iph_ihl:4, ip_ver:4;
    unsigned char         iph_tos;
    unsigned short int    iph_len;
    unsigned short int    iph_ident;
    unsigned char         iph_flags;
    unsigned short int    iph_offset;
    unsigned char         iph_ttl;
    unsigned char         iph_protocol;
    unsigned short int    iph_chksum;
    unsigned int          iph_source;
    unsigned int          iph_dest;
}   t_ip_pack;

typedef struct s_msg_in {
    struct sockaddr_in rec_addr;
    struct iovec io;
    struct msghdr msghdr;
    char buf[512];
}   t_msg_in;

typedef struct s_rt_stats {
    double min;
    double max;
    double sum;
    double sum2;
    u_int16_t pkg_sent;
    u_int16_t pkg_received;
    struct timeval start_time;
}   t_rt_stats;

typedef struct s_stats {

}   t_stats;

typedef struct s_info {
    int sfd;
    int ttl;
    int icmp_size;
    pid_t pid;
    char dst_char[INET_ADDRSTRLEN];
    struct sockaddr_in  dst;
}   t_info;

t_rt_stats g_rt_stats;

struct sockaddr_in get_address(char *input);
int     get_socket();
t_icmp_pack *get_icmp_packet(pid_t pid);
void update_icmp_packet(t_icmp_pack *p);
void    fill_icmp_packet(t_icmp_pack *packet);
u_int16_t compute_checksum(u_int16_t *addr, int count);
void    prepare_destination(struct sockaddr_in *sin);
void    send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin);
void    receive_packet(int sfd);
void    print_trip_stats(char *dest, int ttl, int icmp_size, t_icmp_pack *packet);
void print_execution_summary(char *dst);

void    print_memory(void *memory, unsigned int len);

double get_trip_time(struct timeval tv_begin);
void update_rt_stats(double time);

#endif
