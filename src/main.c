#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include "ping.h"
#include "lib.h"

void	run_requests(int sfd);
void	sig_handler(int signo);
void	prepare_info(char *destination);

//volatile sig_atomic_t g_v = 1;

// TODO switch info.address_info to addrinfo to support both IPv4 and IPv6
int		main(int argv, char *args[])
{
	int i = options(argv, args);
//	printf("%ld\n", sizeof(struct timeval));
//	printf("flags - %x, ttl - %d\n", g_info.options, g_info.ttl);
//    return 0;
//    ft_htons(0x66DF);
	prepare_info(args[argv - 1]);
	//TODO decide between malloc and in-place
	if (signal(SIGALRM, sig_handler) == SIG_ERR)
		exit_with_error(SIGNAL_ERROR);
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		exit_with_error(SIGNAL_ERROR);
	print_execution_intro(args[argv - 1]);
	run_requests(g_info.sfd);
	return (0);
}

void	prepare_info(char *destination)
{
//	g_info.pid = getpid();
//	g_info.sfd = get_socket();
	if (!(g_info.options & T_FLAG))
		g_info.ttl = DEFAULT_TTL;
//	//TODO check if input already an address
	g_info.address_info = get_address(destination);
	inet_ntop(AF_INET, &(g_info.address_info.sin_addr), g_info.dst_char, sizeof(g_info.dst_char));
//    g_info.icmp_data_size = DEFAULT_ICMP_DATA;
	//TODO decide between malloc and in-place
	g_info.icmp_packet = get_icmp_packet();
	g_info.rt_stats = (t_rt_stats *)malloc(sizeof(t_rt_stats));
	ft_memset(g_info.rt_stats, 0, sizeof(*g_info.rt_stats));
	if (gettimeofday(&g_info.rt_stats->start_time, NULL) != 0)
		exit_with_error(GETTIMEOFDAY_ERROR);
	g_info.rt_stats->min = DEFAULT_TIMEOUT * 1000000; // max waiting time
	g_info.rt_stats->seq = 1;
}

void	run_requests(int sfd)
{
	int 		status;
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

	sig_handler(SIGALRM);
	while (1)
	{
		if (recvmsg(sfd, &msg.msghdr, 0) < 0)
			exit_with_error(RECVMSG_ERROR);
		verify_received_packet(&msg);
		if (g_info.options & C_FLAG && g_info.rt_stats->pkg_sent == g_info.count)
		{
			close(g_info.sfd);
			free(g_info.icmp_packet);
			exit(print_execution_summary());
		}
		// TODO think about changing the size of icmp packet
	}
}

void	sig_handler(int signo)
{
	if (signo == SIGALRM)
	{
		update_icmp_packet(g_info.rt_stats->seq, g_info.icmp_packet);
		send_packet(g_info.sfd, g_info.icmp_packet, &g_info.address_info);
		//TODO get rid of duplicating seq and pack_set?
		g_info.rt_stats->seq++;
		alarm(1);
	}
	else if (signo == SIGINT)
	{
		close(g_info.sfd);
		free(g_info.icmp_packet);
		exit(print_execution_summary());
	}
}
