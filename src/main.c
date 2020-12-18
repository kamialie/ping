#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#include "ping.h"
#include "lib.h"

#include <stdio.h>

/*
** global variable control the flow of program
*/
volatile sig_atomic_t g_v = SEND_PACKET;

int		main(int argv, char *args[])
{
	t_info		*info;
	t_msg_in	*msg;

	if ((info = (t_info*)malloc(sizeof(*info))) == NULL)
		exit_with_error(MALLOC_ERROR);
	ft_memset(info, 0, sizeof(*info));
	options(argv, args, &info->options);
	prepare_info(args[argv - 1], info);
	if (signal(SIGALRM, sig_handler) == SIG_ERR)
		exit_with_error(SIGNAL_ERROR);
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		exit_with_error(SIGNAL_ERROR);
	print_execution_intro(args[argv - 1], info->dst_char,
									info->options.icmp_data_size);
	msg = prepare_msg_object(info->icmp_size);
	run_requests(msg, info);
	return (0);
}

void	sig_handler(int signo)
{
	if (signo == SIGALRM)
		g_v = SEND_PACKET;
	else if (signo == SIGINT)
		g_v = EXIT;
}

/*
** recvmsg is set to non-blocking mode
** and will exit with errno set to EINTR
** in case of being interrupted by signal
** and to EAGAIN when did not receive any
** message by the end of timer
*/

void	run_requests(t_msg_in *msg, t_info *info)
{
	if (info->options.options & L_FLAG)
		preload(info);
	while (1)
	{
		if (g_v == SEND_PACKET)
		{
			update_icmp_packet(info->rt_stats->pkg_sent + 1,
									info->icmp_size, info->icmp_packet);
			info->rt_stats->pkg_sent += send_packet(&info->address_info, info);
			g_v = DO_NOTHING;
			alarm(1);
		}
		if (recvmsg(info->sfd_in, &msg->msghdr, 0) < 0)
		{
			if (errno != EINTR && errno != EAGAIN)
				exit_with_error(RECVMSG_ERROR);
		}
		else
			verify_received_packet(msg, info->rt_stats, info);
		if (g_v == EXIT || (info->options.options & C_FLAG &&
				info->rt_stats->pkg_sent == info->options.count))
			return (exit_program(msg, info));
	}
}

void	preload(t_info *info)
{
	while (info->options.preload-- > 0)
	{
		update_icmp_packet(info->rt_stats->pkg_sent + 1,
							info->icmp_size, info->icmp_packet);
		info->rt_stats->pkg_sent += send_packet(&info->address_info, info);
	}
}
