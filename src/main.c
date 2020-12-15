#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include "ping.h"
#include "lib.h"

void	run_requests(t_info *info);
void	sig_handler(int signo);
void	prepare_info(char *input, t_info *info);

volatile sig_atomic_t g_v = 1;

// TODO switch info.address_info to addrinfo to support both IPv4 and IPv6
int		main(int argv, char *args[])
{
	t_info info;
	ft_memset(&info, 0, sizeof(info));
	options(argv, args, &info.options);
//	printf("%ld\n", sizeof(struct timeval));
//	printf("flags - %x, ttl - %d\n", g_info.options, g_info.ttl);
//    return 0;
//    ft_htons(0x66DF);
	prepare_info(args[argv - 1], &info);
	//TODO decide between malloc and in-place
	if (signal(SIGALRM, sig_handler) == SIG_ERR)
		exit_with_error(SIGNAL_ERROR);
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		exit_with_error(SIGNAL_ERROR);
	print_execution_intro(args[argv - 1], info.dst_char);
	run_requests(&info);
	return (0);
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
	//TODO check if input already an address
	info->address_info = get_address(input);
	inet_ntop(AF_INET, &(info->address_info.sin_addr), info->dst_char, sizeof(info->dst_char));
	//TODO decide between malloc and in-place
	info->icmp_packet = get_icmp_packet(info);
	info->rt_stats = (t_rt_stats *)malloc(sizeof(*info->rt_stats));
	ft_memset(info->rt_stats, 0, sizeof(*info->rt_stats));
	if (gettimeofday(&info->rt_stats->tv_start, NULL) != 0)
		exit_with_error(GETTIMEOFDAY_ERROR);
	info->rt_stats->min = DEFAULT_TIMEOUT * 1000000; // max waiting time
}

void	run_requests(t_info *info)
{
	t_msg_in	msg;

	ft_memset(&msg, 0, sizeof(msg));
	msg.io.iov_base = msg.buf;
	msg.io.iov_len = 256; //TODO macro
	msg.msghdr.msg_name = &msg.rec_addr;
	msg.msghdr.msg_namelen = sizeof(msg.rec_addr);
	msg.msghdr.msg_control = msg.buf + 256;
	msg.msghdr.msg_controllen = 256;
	msg.msghdr.msg_iov = &msg.io;
	msg.msghdr.msg_iovlen = 1;

	g_v = SEND_PACKET;
	// TODO think about changing the size of icmp packet
	while (1)
	{
		if (g_v == SEND_PACKET)
		{
			update_icmp_packet(info->rt_stats->pkg_sent + 1, info->icmp_packet);
			info->rt_stats->pkg_sent += send_packet(info->sfd_out, info->icmp_packet, &info->address_info);
			g_v = DO_NOTHING;
			alarm(1);
		}
		else if (g_v == EXIT)
		{
			close(info->sfd_in);
			close(info->sfd_out);
			free(info->icmp_packet);
			//TODO better separate them
			exit(print_execution_summary(info->dst_char, info->rt_stats));
		}
		if (recvmsg(info->sfd_in, &msg.msghdr, 0) < 0)
		{
			// TODO add info about this error codes
			if (errno != EINTR && errno != EAGAIN)
				exit_with_error(RECVMSG_ERROR);
		}
		else
			verify_received_packet(info->pid, info->rt_stats, &msg);
		if (info->options.options & C_FLAG && info->rt_stats->pkg_sent == info->options.count)
		{
			close(info->sfd_in);
			close(info->sfd_out);
			free(info->icmp_packet);
			//TODO better separate them
			exit(print_execution_summary(info->dst_char, info->rt_stats));
		}
	}
}

void 	sig_handler(int signo)
{
	if (signo == SIGALRM)
		g_v = SEND_PACKET;
	else if (signo == SIGINT)
		g_v = EXIT;
}
