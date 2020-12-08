#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include "ping.h"
#include "lib.h"

void run_requests(t_icmp_pack *icmp_packet, t_info *info);
void sig_handler(int signo);

volatile sig_atomic_t g_v = 1;

// TODO switch info.dst to addrinfo to support both IPv4 and IPv6
int	main(int argv, char *args[])
{
    int i;
    t_info info;
    t_icmp_pack *icmp_packet;

    ft_memset(&info, 0, sizeof(t_info));
//    i = options(argv, args, &info);
//    printf("flags - %x, ttl - %d\n", info.flags, info.ttl);
//    return 0;
//    ft_htons(0x66DF);
    g_rt_stats.min = DEFAULT_TIMEOUT * 1000; // max waiting time
    if (gettimeofday(&g_rt_stats.start_time, NULL) != 0) {
        perror("gettimeofday() error");
        exit(2);
    }
    info.pid = getpid();
    info.sfd = get_socket();
    info.ttl = DEFAULT_TTL;
    info.icmp_data_size = DEFAULT_ICMP_DATA;
    info.dst = get_address(args[argv - 1]);
    inet_ntop(AF_INET, &(info.dst.sin_addr), info.dst_char, sizeof info.dst_char);
    icmp_packet = get_icmp_packet(info.pid);
    print_execution_intro(args[argv - 1], &info);
    g_rt_stats.pkg_sent = 0;
    if (signal(SIGALRM, sig_handler) == SIG_ERR)
        exit(2);
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        exit(2);
    run_requests(icmp_packet, &info);
    return (0);
}

void run_requests(t_icmp_pack *icmp_packet, t_info *info)
{
    t_msg_in msg;

    ft_memset(&msg, 0, sizeof(msg));
    msg.io.iov_base = msg.buf;
    msg.io.iov_len = 256; //TODO macro
    msg.msghdr.msg_name = &msg.rec_addr;
    msg.msghdr.msg_namelen = sizeof(msg.rec_addr);
    msg.msghdr.msg_control = msg.buf + 256;
    msg.msghdr.msg_controllen = 256;
    msg.msghdr.msg_iov = &msg.io;
    msg.msghdr.msg_iovlen = 1;

    // TODO multicast ping
    while (1) {
        if (g_v == 1)
        {
            update_icmp_packet(icmp_packet);
            send_packet(info->sfd, icmp_packet, &info->dst);
            if (recvmsg(info->sfd, &msg.msghdr, 0) < 0) {
                perror("revmsg() error");
                exit(-1);
            }
            print_trip_stats(info->dst_char, info->ttl, DEFAULT_ICMP_DATA + sizeof(struct s_icmp_hdr), icmp_packet);
            alarm(1);
            g_v = 0;
        }
        else if (g_v == 2)
        {
            close(info->sfd);
            exit(print_execution_summary(info->dst_char));
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