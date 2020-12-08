#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> // protocol macros

#include <string.h>

#include "ping.h"

int get_socket()
{
    int sfd;

    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        perror("socket() error");
        exit(2);
    }
    // TODO add setsockopt to change ttl
    return (sfd);
}

void send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin) {
    if (sendto(sfd, packet, sizeof(*packet), 0, (struct sockaddr *)sin, sizeof(*sin)) < 0)
    {
        perror("sendto() error");
        exit(2);
    }
    g_rt_stats.pkg_sent++;
}
