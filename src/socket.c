#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h> // protocol macros

#include "ping.h"

int 	get_socket_out(t_options *opt)
{
	int sfd;

	if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		exit_with_error(SOCKET_ERROR);
	if (opt->options & T_FLAG)
		if (setsockopt(sfd, IPPROTO_IP, IP_TTL, &opt->ttl, sizeof(opt->ttl)) < 0)
			exit_with_error(SETSOCKOPT_ERROR);
//    if (g_info.options & B_FLAG)
//        if(setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
//        {
//            perror("setsockopt() error");
//            exit(-1);
//        }
	return (sfd);
}

int 	get_socket_in(void)
{
	int sfd;
	struct timeval time;

	time.tv_sec = 2;
	time.tv_usec = 0;
	if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		exit_with_error(SOCKET_ERROR);
	if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &time, sizeof(time)) < 0)
		exit_with_error(SETSOCKOPT_ERROR);
	return (sfd);
}

//TODO maybe move to packet
int	send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin)
{
	if (sendto(sfd, packet, sizeof(packet->header) + DEFAULT_ICMP_DATA, 0,
			(struct sockaddr *)sin, sizeof(*sin)) < 0)
		exit_with_error(SENDTO_ERROR);
	return (1);
}
