#include <sys/socket.h>
#include <netinet/in.h>

#include "ping.h"

int	get_socket_out(t_options *opt)
{
	int sfd;

	if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		exit_with_error(SOCKET_ERROR);
	if (opt->options & T_FLAG)
		if (setsockopt(sfd, IPPROTO_IP, IP_TTL, &opt->ttl,
										sizeof(opt->ttl)) < 0)
		{
			exit_with_error(SETSOCKOPT_ERROR);
		}
	return (sfd);
}

int	get_socket_in(void)
{
	int				sfd;
	struct timeval	time;

	time.tv_sec = 2;
	time.tv_usec = 0;
	if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		exit_with_error(SOCKET_ERROR);
	if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO,
				(char *)&time, sizeof(time)) < 0)
	{
		exit_with_error(SETSOCKOPT_ERROR);
	}
	return (sfd);
}
