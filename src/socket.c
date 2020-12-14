#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h> // protocol macros

#include "ping.h"

int		get_socket(void)
{
	int sfd;
	int on;

	on = 1;
	//TODO display error about broadcast
	if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		exit_with_error(SOCKET_ERROR);
//    if (g_info.options & B_FLAG)
//        if(setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
//        {
//            perror("setsockopt() error");
//            exit(-1);
//        }
	if (g_info.options & T_FLAG)
		if (setsockopt(sfd, IPPROTO_IP, IP_TTL, &g_info.ttl,
				sizeof(g_info.ttl)) < 0)
			exit_with_error(SETSOCKOPT_ERROR);
	return (sfd);
}

//TODO maybe move to signal handler
void	send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin)
{
	if (sendto(sfd, packet, sizeof(packet->header) + DEFAULT_ICMP_DATA, 0, (struct sockaddr *)&g_info.address_info, sizeof(*sin)) < 0)
		exit_with_error(SENDTO_ERROR);
	g_info.rt_stats->pkg_sent++;
}
