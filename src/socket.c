#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // protocol macros

#include "ping.h"

int get_socket()
{
    int sfd;
    int on;

    on = 1;
    //TODO display error about broadcast
    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        perror("socket() error");
        exit(2);
    }
//    if (g_info.flags & B_FLAG)
//        if(setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
//        {
//            perror("setsockopt() error");
//            exit(-1);
//        }
    if (g_info.flags & T_FLAG)
        if(setsockopt(sfd, IPPROTO_IP, IP_TTL, &g_info.ttl, sizeof(g_info.ttl)) < 0)
        {
            fprintf(stderr, "ft_ping: can't set unicast time-to-live: Invalid argument");
            exit(-1);
        }
    return (sfd);
}

void send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin) {
    if (sendto(sfd, packet, sizeof(*packet), 0, (struct sockaddr *)sin, sizeof(*sin)) < 0)
    {
        perror("sendto() error");
        exit(2);
    }
    g_info.rt_stats->pkg_sent++;
}
