#include <netinet/ip_icmp.h>
#include <stdio.h>

#include "ping.h"

void print_usage()
{
    fprintf(stdout, "Usage: ping [-h] [-c count] [-t ttl] destination\n");
    exit(2);
}

void exit_with_error(int code)
{
	if (code == COUNT_OPTION_ERROR)
		fprintf(stderr, "ft_ping: bad number of packets to transmit.\n");
	if (code == TTL_OPTION_ERROR)
        fprintf(stderr, "ft_ping: ttl out of range.\n");
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
	else if (code == PATTERN_ERROR)
		fprintf(stderr, "ft_ping: patterns must be specified as hex digits. (16 max)\n");
	exit(2);
}

void	print_execution_intro(char *input, char *dst)
{
    printf("PING %s (%s) %d(%ld) bytes of data.\n",
           input, dst, DEFAULT_ICMP_DATA,
           DEFAULT_ICMP_DATA + sizeof(t_icmp_hdr) + sizeof(t_ip_hdr));
}

// TODO output data from incoming packet
//void print_trip_stats(t_icmp_pack *icmp_in, char *address, int ttl)
void print_trip_stats(int ttl, double time, char *address, t_icmp_pack *icmp_in)
{
//    t_icmp_hdr  hdr;
//    char address[INET_ADDRSTRLEN];

//    inet_ntop(rec_addr->sin_family, (void*)&rec_addr->sin_addr, address, INET6_ADDRSTRLEN);
//        hdr = g_info.icmp_packet->header;
	//TODO possibly remove sizeof call
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%#.3g ms\n",
           DEFAULT_ICMP_DATA + (int)sizeof(t_icmp_hdr), address, ft_ntohs(icmp_in->header.seq), ttl, time);
}

void print_trip_error(t_icmp_pack *icmp_in, char *address)
{
    char *error_message;

//    printf("error - %d\n", icmp_in->header.type);
    if (icmp_in->header.type == ICMP_TIMXCEED)
        error_message = "Time to live exceeded";
    else
        error_message = "Unknown error";
    printf("From %s icmp_seq=%d %s\n", address, ft_ntohs(icmp_in->header.seq), error_message);
}

int print_execution_summary(char *dst, t_rt_stats *stats)
{
    // TODO smean
    //sqrt(smean-(mean*mean)

    int status;
    double percent_loss;
    long total_time;

    status = 0;
    total_time = get_trip_time(&stats->tv_start);
    if (stats->pkg_received == 0)
        status = 1;
//    if (rt_stats->pkg_received == 0)
//        percent_loss = 100;
//    else if (rt_stats->pkg_received == rt_stats->pkg_sent)
//        percent_loss = 0;
//    else
//        percent_loss = (double) rt_stats->pkg_received / rt_stats->pkg_sent * 100;
    percent_loss = (double) (stats->pkg_sent - stats->pkg_received) / stats->pkg_sent * 100;
    printf("\n--- %s ping statistics ---\n", dst);
    if (stats->errors)
        printf("%d packets transmitted, %d received, %+d errors, %.0f%% packet loss, time %#.5gms\n",
               stats->pkg_sent, stats->pkg_received, stats->errors, percent_loss, total_time / 1000.0);
    else
    {
        printf("%d packets transmitted, %d received, %.0f%% packet loss, time %#.5gms\n",
               stats->pkg_sent, stats->pkg_received, percent_loss, total_time / 1000.0);
        if (percent_loss != 100)
            printf("rtt min/avg/max/mdev = %.5g/%.5g/%.5g/%.5g ms\n",
                   stats->min / 1000.0, stats->max / 1000.0, stats->sum / 1000.0 / stats->pkg_received,0.0);
    }
    return status;
}

/*
 * for debugging
 */
void    print_memory(void *memory, unsigned int len)
{
    unsigned int  i;
    unsigned char *buf;

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
