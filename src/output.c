#include <sys/time.h>

#include "ping.h"

void print_usage()
{
    fprintf(stdout, "Usage: ping destination");
}

void print_execution_intro(char *dst, t_info *info)
{
    printf("PING %s (%s) %d(%ld) bytes of data.\n",
           dst, info->dst_char, info->icmp_data_size,
           info->icmp_data_size + sizeof(t_icmp_hdr) + sizeof(t_ip_pack));
}

void print_trip_stats(char *dst, int ttl, int icmp_size, t_icmp_pack *icmp_packet)
{
    double      time_milli_s;
    long        time_micro_s;
    t_icmp_hdr  hdr;

    hdr = icmp_packet->header;
    time_micro_s = get_trip_time(icmp_packet->tv);
    time_milli_s = time_micro_s / 1000.0;
    update_rt_stats(time_micro_s);
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%#.3g ms\n",
           icmp_size, dst, ntohs(hdr.seq), ttl, time_milli_s);
}

int print_execution_summary(char *dst)
{
    // TODO smean
    //sqrt(smean-(mean*mean)

    int status;
    double percent_loss;
    long total_time;

    status = 0;
    total_time = get_trip_time(g_rt_stats.start_time);
    if (g_rt_stats.pkg_received == 0)
        status = 1;
    if (g_rt_stats.pkg_received == g_rt_stats.pkg_sent)
        percent_loss = 0;
    else
        percent_loss = (double) g_rt_stats.pkg_received / g_rt_stats.pkg_sent * 100;
    printf("\n--- %s ping statistics ---\n", dst);
    printf("%d packets transmitted, %d received, %.0f%% packet loss, time %#.5gms\n",
           g_rt_stats.pkg_sent, g_rt_stats.pkg_received, percent_loss, total_time / 1000.0);
    printf("rtt min/avg/max/mdev = %.5g/%.5g/%.5g/%.5g ms\n",
           g_rt_stats.min / 1000.0, g_rt_stats.max / 1000.0, g_rt_stats.sum / 1000.0 / g_rt_stats.pkg_received,0.0);
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
