#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include "ping.h"
#include "lib.h"

/*
** getting sockets for sending and receiving packets
** checking options passed by user
** getting destination address
** preparing non-changing parts of icmp packet
** preparing struct for gathering rt stats
*/

void		prepare_info(char *input, t_info *info)
{
	info->pid = getpid();
	info->sfd_out = get_socket_out(&info->options);
	info->sfd_in = get_socket_in();
	if (!(info->options.options & T_FLAG))
		info->options.ttl = DEFAULT_TTL;
	if (!(info->options.options & S_FLAG))
		info->options.icmp_data_size = DEFAULT_ICMP_DATA_SIZE;
	info->icmp_size = (int)sizeof(t_icmp_hdr) + info->options.icmp_data_size;
	get_address(input, info);
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
