#include "ping.h"
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>

void run_requests(t_icmp_pack *icmp_packet, t_info *info);
void sig_handler(int signo);

volatile sig_atomic_t g_v = 1;

// TODO switch info.dst to addrinfo to support both IPv4 and IPv6
int	main(int argv, char *args[])
{
    t_info info;
    t_icmp_pack *icmp_packet;

    (void)argv;
    g_rt_stats.min = DEFAULT_TIMEOUT * 1000; // max waiting time
    if (gettimeofday(&g_rt_stats.start_time, NULL) != 0) {
        perror("gettimeofday() error");
        exit(1);
    }

    info.pid = getpid();
    info.sfd = get_socket();
    info.ttl = DEFAULT_TTL;
//    info.dest_char = "192.168.1.1";
    info.dst = get_address(args[1]);
    inet_ntop(AF_INET, &(info.dst.sin_addr), info.dst_char, sizeof info.dst_char);
    icmp_packet = get_icmp_packet(info.pid);
//    prepare_destination(&info.dst);

    run_requests(icmp_packet, &info);
    close(info.sfd);

    printf("\n");
    printf("size of ip packet - %ld\n", sizeof(t_ip_pack));
    printf("size of tv - %ld\n", sizeof(struct timeval));
    printf("size of icmp header - %ld\n", sizeof(t_icmp_hdr));
    printf("size of icmp packet - %ld\n", sizeof(t_icmp_pack));
    printf("size of int - %ld\n", sizeof(int));

    return (0);
}

void run_requests(t_icmp_pack *icmp_packet, t_info *info) {
    ssize_t bytes;
    t_msg_in msg;

    memset(&msg, 0, sizeof(msg));
    msg.io.iov_base = msg.buf;
    msg.io.iov_len = 256; //TODO macro
    msg.msghdr.msg_name = &msg.rec_addr;
    msg.msghdr.msg_namelen = sizeof(msg.rec_addr);
    msg.msghdr.msg_control = msg.buf + 256;
    msg.msghdr.msg_controllen = 256;
    msg.msghdr.msg_iov = &msg.io;
    msg.msghdr.msg_iovlen = 1;

    g_rt_stats.pkg_sent = 0;
    if (signal(SIGALRM, sig_handler) == SIG_ERR)
        exit(1);
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        exit(1);
    while (1) {
        if (g_v == 1)
        {
            update_icmp_packet(icmp_packet);
            send_packet(info->sfd, icmp_packet, &info->dst);
            //    receive_packet(info->sfd);
            if ((bytes = recvmsg(info->sfd, &msg.msghdr, 0)) < 0) {
                perror("revmsg() error");
                exit(-1);
            }
    //        else {
    //            printf("received - %ld\n", bytes);
    //        }

            print_trip_stats(info->dst_char, info->ttl, DEFAULT_ICMP_DATA + sizeof(struct s_icmp_hdr), icmp_packet);
            alarm(1);
            g_v = 0;
        }
        else if (g_v == 2)
        {
            print_execution_summary(info->dst_char);
            exit(1);
        }
    }
}

void sig_handler(int signo)
{
    if (signo == SIGALRM)
        g_v = 1;
    else if (signo == SIGINT)
        g_v = 2;
}