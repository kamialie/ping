#include <sys/time.h>

#include "ping.h"

void print_usage() {

}

void print_trip_stats(char *dst, int ttl, int icmp_size, t_icmp_pack *icmp_packet) {
    double      time;
    t_icmp_hdr  hdr;


//    t_icmp_pack *icmp_in;
//    icmp_in = (t_icmp_pack *)(20 + buf);

//    printf("size of message - %ld\n", msg.msg_iov[0].iov_len);
//    printf("incoming address - %d\n", ntohl(*((int*)&rec_addr.sin_addr)));
//    printf("icmp message type - %d\n", hdr.type);
//    printf("icmp message seq - %d\n", ntohs(hdr.seq));
//    printf("icmp message id - %d\n", ntohs(hdr.id));

    hdr = icmp_packet->header;
    time = get_trip_time(icmp_packet->tv);
    update_rt_stats(time);
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3g ms\n", icmp_size, dst, ntohs(hdr.seq), ttl, time);
}

void print_execution_summary(char *dst) {

    // TODO rtt
    //--- 192.168.1.1 ping statistics ---
    //1 packets transmitted, 1 received, 0% packet loss, time 0ms
    //rtt min/avg/max/mdev = 11.162/11.162/11.162/0.000 ms

    // TODO smean
    //sqrt(smean-(mean*mean)

    double percent_loss;
    double total_time = 0;

    total_time = get_trip_time(g_rt_stats.start_time);
    if (g_rt_stats.pkg_received == g_rt_stats.pkg_sent)
        percent_loss = 0;
    else
        percent_loss = (double) g_rt_stats.pkg_received / g_rt_stats.pkg_sent * 100;
    printf("\n--- %s ping statistics ---\n", dst);
    printf("%d packets transmitted, %d received, %.0f%% packet loss, time %.3gms\n",
           g_rt_stats.pkg_sent, g_rt_stats.pkg_received, percent_loss, total_time);
    printf("rtt min/avg/max/mdev = %.5g/%.5g/%.5g/%.5g ms\n",
           g_rt_stats.min, g_rt_stats.max, g_rt_stats.sum / g_rt_stats.pkg_received,0.0);
}

/*
 * for debugging
 */
void    print_memory(void *memory, unsigned int len) {
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
