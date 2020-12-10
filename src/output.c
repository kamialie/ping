#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

#include "ping.h"

void print_usage()
{
    fprintf(stdout, "Usage: ping [-h] [-c count] [-t ttl] destination\n");
    exit(2);
}

void print_error(int code)
{
    if (code == 1)
        fprintf(stdout, "ft_ping: bad number of packets to transmit.\n");
    exit(2);
}

void print_execution_intro(char *dst)
{
    printf("PING %s (%s) %d(%ld) bytes of data.\n",
           dst, g_info.dst_char, DEFAULT_ICMP_DATA,
           DEFAULT_ICMP_DATA + sizeof(t_icmp_hdr) + sizeof(t_ip_hdr));
}

// TODO output data from incoming packet
void print_trip_stats(t_icmp_pack *icmp_in, char *address, int ttl)
{
    double      time_milli_s;
    long        time_micro_s;
//    t_icmp_hdr  hdr;
//    char address[INET_ADDRSTRLEN];

//    inet_ntop(rec_addr->sin_family, (void*)&rec_addr->sin_addr, address, INET6_ADDRSTRLEN);
    time_micro_s = get_trip_time(icmp_in->tv);
    time_milli_s = time_micro_s / 1000.0;
    update_rt_stats(time_micro_s);
//        hdr = g_info.icmp_packet->header;
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%#.3g ms\n",
           DEFAULT_ICMP_DATA + (int)sizeof(t_icmp_hdr), address, ntohs(icmp_in->header.seq), ttl, time_milli_s);
}

void print_trip_error(t_icmp_pack *icmp_in, char *address)
{
    char *error_message;

    printf("error - %d\n", icmp_in->header.type);
    if (icmp_in->header.type == ICMP_TIMXCEED)
        error_message = "Time to live exceeded";
    else
        error_message = "Unknown error";
    printf("From %s icmp_seq=%d %s\n", address, ntohs(icmp_in->header.seq), error_message);
}

int print_execution_summary(void)
{
    // TODO smean
    //sqrt(smean-(mean*mean)

    int status;
    double percent_loss;
    long total_time;
    t_rt_stats *rt_stats;

    status = 0;
    rt_stats = g_info.rt_stats;
    total_time = get_trip_time(g_info.rt_stats->start_time);
    if (rt_stats->pkg_received == 0)
        status = 1;
//    if (rt_stats->pkg_received == 0)
//        percent_loss = 100;
//    else if (rt_stats->pkg_received == rt_stats->pkg_sent)
//        percent_loss = 0;
//    else
//        percent_loss = (double) rt_stats->pkg_received / rt_stats->pkg_sent * 100;
    percent_loss = (double) (rt_stats->pkg_sent - rt_stats->pkg_received) / rt_stats->pkg_sent * 100;
    printf("\n--- %s ping statistics ---\n", g_info.dst_char);
    if (rt_stats->errors)
        printf("%d packets transmitted, %d received, %+d errors, %.0f%% packet loss, time %#.5gms\n",
               rt_stats->pkg_sent, rt_stats->pkg_received, rt_stats->errors, percent_loss, total_time / 1000.0);
    else
    {
        printf("%d packets transmitted, %d received, %.0f%% packet loss, time %#.5gms\n",
               rt_stats->pkg_sent, rt_stats->pkg_received, percent_loss, total_time / 1000.0);
        if (percent_loss != 100)
            printf("rtt min/avg/max/mdev = %.5g/%.5g/%.5g/%.5g ms\n",
                   rt_stats->min / 1000.0, rt_stats->max / 1000.0, rt_stats->sum / 1000.0 / rt_stats->pkg_received,0.0);
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
    while (i < len) {
        printf("%02x", buf[i]);
        i++;
        if (i % 16 == 0)
            printf("\n%0#6x: ", i);
        else if (i % 2 == 0)
            printf(" ");
    }
    printf("\n");
}
