#include <string.h>
#include <arpa/inet.h>

// icmp packet types
#include <netinet/ip_icmp.h>
#include <sys/time.h>

#include "ping.h"
#include "lib.h"

#include <stdio.h>

static void fill_icmp_pad(int patternlen, unsigned char *pattern, void *pad)
{
	int i;
	int len;
	unsigned char *icmp_pad;

	len = patternlen / 2;
	if (patternlen % 2 != 0)
		len++;
	icmp_pad = pad;
	i = 0;
	while (i < DEFAULT_ICMP_DATA)
		*icmp_pad++ = pattern[i++ % len];
}

t_icmp_pack *get_icmp_packet(t_info *info)
{
    size_t len;
    t_icmp_pack *p;

    len = sizeof(t_icmp_hdr) + DEFAULT_ICMP_DATA;
    if ((p = (t_icmp_pack *)malloc(sizeof(char) * len)) == NULL)
    	exit_with_error(MALLOC_ERROR);
    ft_memset(p, 0, len);
	p->header.type = ICMP_ECHO;
	p->header.code = 0;
    p->header.id = ft_htons(info->pid);
    if (info->options.options & P_FLAG)
	{
		fill_icmp_pad(info->options.patternlen,
				(unsigned char*)&info->options.pattern, &p->pad);
	}
    return p;
}

void update_icmp_packet(int seq, t_icmp_pack *p)
{
    p->header.seq = ft_htons(seq);
    if (gettimeofday(&p->tv, NULL) != 0)
		exit_with_error(GETTIMEOFDAY_ERROR);
    p->header.chksum = 0;
    p->header.chksum = compute_checksum((u_int16_t *)p, sizeof(p->header) + DEFAULT_ICMP_DATA);
}

int	send_packet(int sfd, t_icmp_pack *packet, struct sockaddr_in *sin)
{
	if (sendto(sfd, packet, sizeof(packet->header) + DEFAULT_ICMP_DATA, 0,
			   (struct sockaddr *)sin, sizeof(*sin)) < 0)
		exit_with_error(SENDTO_ERROR);
	return (1);
}

// TODO need ntohs
void verify_received_packet(int pid, t_rt_stats *stats, t_msg_in *msg)
{
    t_icmp_pack *icmp_in;
    t_ip_hdr *ip_hdr;
    char address[INET_ADDRSTRLEN];

    ip_hdr = (t_ip_hdr *)msg->io.iov_base;
    icmp_in = (t_icmp_pack *)((char *)msg->io.iov_base + sizeof(t_ip_hdr));
    inet_ntop(msg->rec_addr.sin_family, (void*)&msg->rec_addr.sin_addr, address, INET6_ADDRSTRLEN);
    // TODO check previous commits on receiving self echo requests
    if (icmp_in->header.type == ICMP_ECHO)
        return;
    if (icmp_in->header.type != ICMP_ECHOREPLY)
    {
        stats->errors++;
        print_trip_error(icmp_in, address);
        return;
    }
	if (compute_checksum((u_int16_t *)icmp_in, sizeof(t_icmp_hdr) + DEFAULT_ICMP_DATA))
        return;
    if (pid != ft_htons(icmp_in->header.id))
        return;
    stats->pkg_received++;
	print_trip_stats(ip_hdr->iph_ttl, update_rt_stats(&icmp_in->tv, stats), address, icmp_in);
}
