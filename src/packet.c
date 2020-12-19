#include <string.h>
#include <arpa/inet.h>

#include <netinet/ip_icmp.h>
#include <sys/time.h>

#include "ping.h"
#include "lib.h"

static void	fill_icmp_pad(int patternlen, const unsigned char *pattern,
											void *pad, int icmp_data_size)
{
	int				i;
	int				len;
	unsigned char	*icmp_pad;

	len = patternlen / 2;
	if (patternlen % 2 != 0)
		len++;
	icmp_pad = pad;
	icmp_data_size -= sizeof(struct timeval);
	i = 0;
	while (i < icmp_data_size)
		*icmp_pad++ = pattern[i++ % len];
}

t_icmp_pack	*get_icmp_packet(t_info *info)
{
	t_icmp_pack *p;

	if ((p = (t_icmp_pack *)malloc(sizeof(char) * info->icmp_size)) == NULL)
		exit_with_error(MALLOC_ERROR);
	ft_memset(p, 0, info->icmp_size);
	p->header.type = ICMP_ECHO;
	p->header.code = 0;
	p->header.id = ft_htons(info->pid);
	if (info->options.options & P_FLAG)
	{
		fill_icmp_pad(info->options.patternlen,
				(unsigned char*)&info->options.pattern,
				&p->pad, info->options.icmp_data_size);
	}
	return (p);
}

void		update_icmp_packet(int seq, int icmp_size, t_icmp_pack *p)
{
	p->header.seq = ft_htons(seq);
	if (icmp_size >= ICMP_MINIMUM_SIZE)
	{
		if (gettimeofday(&p->tv, NULL) != 0)
			exit_with_error(GETTIMEOFDAY_ERROR);
	}
	p->header.chksum = 0;
	p->header.chksum = compute_checksum((u_int16_t *)p, icmp_size);
}

int			send_packet(struct sockaddr_in *sin, t_info *info)
{
	if (sendto(info->sfd_out, info->icmp_packet, info->icmp_size, 0,
							(struct sockaddr *)sin, sizeof(*sin)) < 0)
	{
		exit_with_error(SENDTO_ERROR);
	}
	return (1);
}

void		verify_received_packet(t_msg_in *msg, t_rt_stats *stats,
															t_info *info)
{
	t_icmp_pack	*icmp_in;
	t_ip_hdr	*ip_hdr;
	char		address[INET_ADDRSTRLEN];
	t_ts		trip_stats;

	ip_hdr = (t_ip_hdr *)msg->io.iov_base;
	icmp_in = (t_icmp_pack *)((char *)msg->io.iov_base + sizeof(t_ip_hdr));
	inet_ntop(msg->rec_addr.sin_family, (void*)&msg->rec_addr.sin_addr,
												address, INET6_ADDRSTRLEN);
	trip_stats = (t_ts) {ip_hdr->iph_ttl, info->icmp_size,
		ft_ntohs(icmp_in->header.seq), update_rt_stats(&icmp_in->tv, stats)};
	if (icmp_in->header.type == ICMP_ECHO)
		return ;
	if (icmp_in->header.type != ICMP_ECHOREPLY)
	{
		print_error_message(address, icmp_in, info);
		return ;
	}
	if (compute_checksum((u_int16_t *)icmp_in, info->icmp_size) ||
								info->pid != ft_ntohs(icmp_in->header.id))
		return ;
	stats->pkg_received++;
	print_trip_stats(&trip_stats, address);
}
