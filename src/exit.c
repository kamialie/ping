#include <unistd.h>
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