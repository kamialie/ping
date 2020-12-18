#include <unistd.h>
#include <stdio.h>

#include "ping.h"

void	print_usage(void)
{
	fprintf(stdout, "Usage: ping [-h] [-c count] [-t ttl] [-l] pattern "
								"[-p] preload [-s] size destination\n");
	exit(2);
}

void	exit_program(t_msg_in *msg, t_info *info)
{
	print_execution_summary(info->icmp_size, info->dst_char, info->rt_stats);
	if (info->rt_stats->pkg_sent != info->rt_stats->pkg_received)
		exit(2);
	close(info->sfd_in);
	close(info->sfd_out);
	free(msg->io.iov_base);
	free(msg);
	free(info->icmp_packet);
	free(info->rt_stats);
}

void	exit_with_error(int code)
{
	if (code == C_FLAG)
		fprintf(stderr, "ft_ping: bad number of packets to transmit.\n");
	if (code == T_FLAG)
		fprintf(stderr, "ft_ping: ttl out of range.\n");
	else if (code == L_FLAG)
		fprintf(stderr, "ft_ping: preload out of range.\n");
	else if (code == S_FLAG)
		fprintf(stderr, "ft_ping: icmp data size out of range.\n");
	else if (code == P_FLAG)
		fprintf(stderr, "ft_ping: patterns must be specified as hex digits.\n");
	else if (code == RECVMSG_ERROR)
		fprintf(stderr, "ft_ping: recvmsg() error.\n");
	else if (code == GETTIMEOFDAY_ERROR)
		fprintf(stderr, "ft_ping: gettimeofday() error.\n");
	else if (code == SIGNAL_ERROR)
		fprintf(stderr, "ft_ping: signal() error.\n");
	else if (code == MALLOC_ERROR)
		fprintf(stderr, "ft_ping: malloc() error.\n");
	else if (code == SOCKET_ERROR)
		fprintf(stderr, "ft_ping: socket() error.\n");
	else if (code == SENDTO_ERROR)
		fprintf(stderr, "ft_ping: sendto() error.\n");
	else if (code == SETSOCKOPT_ERROR)
		fprintf(stderr, "ft_ping: setsockopt() error.\n");
	exit(2);
}
