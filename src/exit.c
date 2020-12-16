#include <unistd.h>
#include <stdio.h>

#include "ping.h"

void exit_program(t_info *info)
{
	close(info->sfd_in);
	close(info->sfd_out);
	free(info->icmp_packet);
	print_execution_summary(info->dst_char, info->rt_stats);
	//TODO better check return statuses
	if (info->rt_stats->pkg_sent != info->rt_stats->pkg_received)
		exit(2);
	exit(0);
}

void exit_with_error(int code)
{
	if (code == C_FLAG)
		fprintf(stderr, "ft_ping: bad number of packets to transmit.\n");
	if (code == T_FLAG)
		fprintf(stderr, "ft_ping: ttl out of range.\n");
	else if (code == L_FLAG)
		fprintf(stderr, "ft_ping: preload out of range\n");
	else if (code == S_FLAG)
		fprintf(stderr, "ft_ping: icmp data size out of range\n");
	else if (code == P_FLAG)
		fprintf(stderr, "ft_ping: patterns must be specified as hex digits. (16 max)\n");
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
		fprintf(stderr, "ft_ping: setsockopt() error\n");
	exit(2);
}
