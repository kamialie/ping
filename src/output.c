#include <stdio.h>

#include "ping.h"

void	print_execution_intro(char *input, char *dst, int icmp_data_size)
{
	printf("PING %s (%s) %d(%ld) bytes of data.\n",
			input, dst, icmp_data_size,
			icmp_data_size + sizeof(t_icmp_hdr) + sizeof(t_ip_hdr));
}

void	print_trip_stats(t_ts *trip_stats, char *address)
{
	int icmp_size;

	icmp_size = trip_stats->icmp_size;
	if (icmp_size >= ICMP_MINIMUM_SIZE)
	{
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%#.3g ms\n",
									icmp_size, address, trip_stats->seq,
									trip_stats->ttl, trip_stats->time);
	}
	else
	{
		printf("%d bytes from %s: icmp_seq=%d ttl=%d\n",
				icmp_size, address, trip_stats->seq, trip_stats->ttl);
	}
}

void	print_error_message(char *address, t_icmp_pack *icmp_in, t_info *info)
{
	info->rt_stats->errors++;
	if (info->options.options & V_FLAG)
		printf("From %s: type = %d, code - %d\n",
				address, icmp_in->header.type, icmp_in->header.code);
}

void	print_execution_summary(int icmp_size, char *dst, t_rt_stats *stats)
{
	double	percent_loss;
	long	total_time;

	total_time = get_trip_time(&stats->tv_start);
	percent_loss = (double)(stats->pkg_sent - stats->pkg_received) /
													stats->pkg_sent * 100;
	printf("\n--- %s ping statistics ---\n", dst);
	if (stats->errors)
		printf("%d packets transmitted, %d received, %+d errors,"
							"%.0f%% packet loss, time %#.5gms\n",
							stats->pkg_sent, stats->pkg_received,
							stats->errors, percent_loss, total_time / 1000.0);
	else
	{
		printf("%d packets transmitted, %d received,"
					"%.0f%% packet loss, time %#.5gms\n",
					stats->pkg_sent, stats->pkg_received,
					percent_loss, total_time / 1000.0);
		if (percent_loss != 100 && icmp_size >= ICMP_MINIMUM_SIZE)
			printf("rtt min/avg/max/mdev = %.5g/%.5g/%.5g ms\n",
						stats->min / 1000.0, stats->max / 1000.0,
						stats->sum / 1000.0 / stats->pkg_received);
	}
}

/*
** for debugging
*/

void	print_memory(void *memory, unsigned int len)
{
	unsigned int	i;
	unsigned char	*buf;

	i = 0;
	buf = (unsigned char *)memory;
	printf("0x0000: ");
	while (i < len)
	{
		printf("%02x", buf[i]);
		if (i % 16 == 0)
			printf("\n%0#6x: ", i);
		else if (i % 2 == 0)
			printf(" ");
		i++;
	}
	printf("\n");
}
