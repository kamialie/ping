#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

// icmp packet types
#include <netinet/ip_icmp.h>
#include <sys/time.h>

#include "ping.h"

t_icmp_pack *get_icmp_packet(pid_t pid) {
    size_t len;
    t_icmp_pack *p;

    len = DEFAULT_ICMP_DATA;
    if ((p = (t_icmp_pack *)malloc(sizeof(char) * len)) == NULL)
    {
        perror("malloc() error");
        exit(1);
    }
    memset(p, 0, sizeof(len));
    p->header.type = ICMP_ECHO;
    p->header.code = 0;
    p->header.id = htons(pid);

//    printf("size of packet - %ld\n", sizeof(*p));
//    printf("icmp message type - %d\n", p->header.type);
//    printf("icmp message seq - %d\n", p->header.seq);
//    printf("icmp message chksum - %d\n", p->header.chksum);
//    print_memory(p, sizeof(*p));

    return p;
}

void update_icmp_packet(t_icmp_pack *p) {
    p->header.seq = htons(g_rt_stats.pkg_sent);
    if (gettimeofday(&p->tv, NULL) != 0) {
        perror("gettimeofday() error");
        exit(1);
    }
    p->header.chksum = 0;
    p->header.chksum = htons(compute_checksum((u_int16_t *)p, sizeof(*p)));
}
