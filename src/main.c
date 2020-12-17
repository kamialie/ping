#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#include "ping.h"
#include "lib.h"

#include <stdio.h>

void	run_requests(t_info *info);
void	sig_handler(int signo);
void	prepare_info(char *input, t_info *info);
t_msg_in *prepare_msg_object(int icmp_size);

/*
** global variable control the flow of program
*/
volatile sig_atomic_t g_v = SEND_PACKET;

int		main(int argv, char *args[])
{
	t_info info;

	ft_memset(&info, 0, sizeof(info));
	options(argv, args, &info.options);
	prepare_info(args[argv - 1], &info);
	if (signal(SIGALRM, sig_handler) == SIG_ERR)
		exit_with_error(SIGNAL_ERROR);
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		exit_with_error(SIGNAL_ERROR);
	print_execution_intro(args[argv - 1], info.dst_char,
									info.options.icmp_data_size);
	run_requests(&info);
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
** getting sockets for sending and receiving packets
** checking options passed by user
** getting destination address
** preparing non-changing parts of icmp packet
** preparing struct for gathering rt stats
*/

void	prepare_info(char *input, t_info *info)
{
	info->pid = getpid();
	info->sfd_out = get_socket_out(&info->options);
	info->sfd_in = get_socket_in();
	if (!(info->options.options & T_FLAG))
		info->options.ttl = DEFAULT_TTL;
	if (!(info->options.options & S_FLAG))
		info->options.icmp_data_size = DEFAULT_ICMP_DATA_SIZE;
	info->icmp_size = (int)sizeof(t_icmp_hdr) + info->options.icmp_data_size;
	info->address_info = get_address(input);
	inet_ntop(AF_INET, &(info->address_info.sin_addr),
				info->dst_char, sizeof(info->dst_char));
	info->icmp_packet = get_icmp_packet(info);
	if ((info->rt_stats = malloc(sizeof(*info->rt_stats))) == NULL)
		exit_with_error(MALLOC_ERROR);
	ft_memset(info->rt_stats, 0, sizeof(*info->rt_stats));
	if (gettimeofday(&info->rt_stats->tv_start, NULL) != 0)
		exit_with_error(GETTIMEOFDAY_ERROR);
	info->rt_stats->min = DEFAULT_TIMEOUT * 1000000;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
/*
** recvmsg is set to non-blocking mode
** and will exit with errno set to EINTR
** in case of being interrupted by signal
** and to EAGAIN when did not recieve any
** message by the end of timer
*/

void	run_requests(t_info *info)
{
	t_msg_in	*msg;

	msg = prepare_msg_object(info->icmp_size);
	if (info->options.options & L_FLAG)
	{
		while (info->options.preload-- > 0)
		{
			update_icmp_packet(info->rt_stats->pkg_sent + 1, info->icmp_size, info->icmp_packet);
			info->rt_stats->pkg_sent += send_packet(&info->address_info, info);
		}
	}
	while (1)
	{
		if (g_v == SEND_PACKET)
		{
			update_icmp_packet(info->rt_stats->pkg_sent + 1, info->icmp_size, info->icmp_packet);
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
		if (g_v == EXIT)
			return (exit_program(msg, info));
		if (info->options.options & C_FLAG && info->rt_stats->pkg_sent == info->options.count)
			return (exit_program(msg, info));
	}
}
#pragma clang diagnostic pop

t_msg_in	*prepare_msg_object(int icmp_size)
{
	t_msg_in *msg;

	if ((msg = (t_msg_in *)malloc(sizeof(*msg))) == NULL)
		exit_with_error(MALLOC_ERROR);
	ft_memset(msg, 0, sizeof(*msg));
	msg->io.iov_len = icmp_size + sizeof(t_ip_hdr);
	if ((msg->io.iov_base = malloc(sizeof(char) * msg->io.iov_len)) == NULL)
		exit_with_error(MALLOC_ERROR);
	msg->msghdr.msg_name = &msg->rec_addr;
	msg->msghdr.msg_namelen = sizeof(msg->rec_addr);
	msg->msghdr.msg_control = NULL;
	msg->msghdr.msg_controllen = 0;
	msg->msghdr.msg_iov = &msg->io;
	msg->msghdr.msg_iovlen = 1;
	return (msg);
}
