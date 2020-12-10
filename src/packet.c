#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

// icmp packet types
#include <netinet/ip_icmp.h>
#include <sys/time.h>

#include "ping.h"
#include "lib.h"

t_icmp_pack *get_icmp_packet()
{
    size_t len;
    t_icmp_pack *p;

    len = sizeof(t_icmp_hdr) + g_info.icmp_data_size;
    if ((p = (t_icmp_pack *)malloc(sizeof(char) * len)) == NULL)
    {
        fprintf(stderr, "ft_ping: malloc() error");
        exit(2);
    }
    ft_memset(p, 0, len);
    p->header.type = ICMP_ECHO;
    p->header.code = 0;
    p->header.id = ft_htons(g_info.pid);
    return p;
}

void update_icmp_packet(int seq, t_icmp_pack *p)
{
    p->header.seq = ft_htons(seq);
    if (gettimeofday(&p->tv, NULL) != 0) {
        fprintf(stderr, "ft_ping: gettimeofday() error");
        exit(1);
    }
    p->header.chksum = 0;
    p->header.chksum = compute_checksum((u_int16_t *)p, sizeof(*p));
}

// TODO need ntohs
int verify_received_packet(t_msg_in *msg)
{
    t_icmp_pack *icmp_in;
    t_ip_hdr *ip_hdr;
    char address[INET_ADDRSTRLEN];

    ip_hdr = (t_ip_hdr *)msg->io.iov_base;
    icmp_in = (t_icmp_pack *)((char *)msg->io.iov_base + sizeof(t_ip_hdr));
//    print_memory((void*)&ip_packet->iph_source, sizeof(ip_packet->iph_source));
    inet_ntop(msg->rec_addr.sin_family, (void*)&msg->rec_addr.sin_addr, address, INET6_ADDRSTRLEN);
//    printf("orig - %d in - %d\n", g_info.rt_stats->seq, ft_htons(icmp_in->header.seq));
    // TODO check previos commits on receiving self echo requests
//    if (icmp_in->header.type == ICMP_ECHO)
//        return 1;
    if (icmp_in->header.type != ICMP_ECHOREPLY)
    {
        g_info.rt_stats->errors++;
        print_trip_error(icmp_in, address);
    }
    if (compute_checksum((u_int16_t *)icmp_in, sizeof(*icmp_in)))
        return 1;
    if (g_info.pid != ft_htons(icmp_in->header.id))
        return 1;

    g_info.rt_stats->pkg_received++;
    print_trip_stats(icmp_in, address, ip_hdr->iph_ttl);
//    print_trip_stats();
    return icmp_in->header.type;
}
