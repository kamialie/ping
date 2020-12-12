#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h> // protocol macros
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include "ping.h"
#include "lib.h"

struct sockaddr_in get_address(char *input) {
    struct sockaddr_in *ipv4f;
    struct addrinfo hints, *res, *p;
    char ipstr[INET6_ADDRSTRLEN];
    int status;

    ft_memset(&hints, 0, sizeof hints);
//    hints.ai_family = AF_UNSPEC; // or force to use AF_INET or AF_INET6
//    hints.ai_socktype = SOCK_STREAM; // resolving DNS with tcp???
//    hints.ai_flags |= AI_CANONNAME;
    hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_flags = AI_CANONNAME;

    if ((status = getaddrinfo(input, 0, &hints, &res)) != 0)
    {
        fprintf(stderr, "ft_ping: %s: %s\n", input, gai_strerror(status));
        exit(2);
    }

#include <sys/time.h>
#include <errno.h>
    p = res;
	int sfd;
	if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		exit_with_error(SOCKET_ERROR);
		//TODO FQDN not working!
    while (p != NULL)
    {
        if (p->ai_family == AF_INET) // IPv4
        {
//            printf("canin flag - %x\n", AI_CANONNAME);
//            printf("canon - %s\n", p->ai_canonname);
//			printf("KAKA\n");
//			g_info.address_info =  *(struct sockaddr_in *)p->ai_addr;
//			g_info.icmp_packet = get_icmp_packet();
//			g_info.rt_stats = (t_rt_stats *)malloc(sizeof(t_rt_stats));
//			ft_memset(g_info.rt_stats, 0, sizeof(*g_info.rt_stats));
//			if (gettimeofday(&g_info.rt_stats->start_time, NULL) != 0)
//				exit_with_error(GETTIMEOFDAY_ERROR);
//			update_icmp_packet(g_info.rt_stats->seq, g_info.icmp_packet);
			printf("KAKA\n");
			printf("AF_INET6 - %d, value - %d\n", AF_INET6, p->ai_family);
			printf("addrlen - %d\n", p->ai_addrlen);
			print_memory(p->ai_addr, p->ai_addrlen);
			if (sendto(sfd, "kek", 3, 0, p->ai_addr, p->ai_addrlen) < 0) {
				perror("sendto");
				printf("%d (%s)\n", errno, strerror(errno));
			}
			exit(1);
            return *(struct sockaddr_in *)p->ai_addr;
        }
    }
//    for(p = res;p != NULL; p = p->ai_next) {
//        void *addr;
//        char *ipver;
//
//        // get the pointer to the address itself,
//        // different fields in IPv4 and IPv6:
//        if (p->ai_family == AF_INET) { // IPv4
//            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
//            addr = &(ipv4->sin_addr);
//            ipver = "IPv4";
//        } else { // IPv6
//            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
//            addr = &(ipv6->sin6_addr);
//            ipver = "IPv6";
//        }
//
//        // convert the IP to a string and print it:
//        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
//        printf("canon - %s\n", p->ai_canonname);
//        printf("  %s: %s\n", ipver, ipstr);
//    }
    fprintf(stderr, "kek\n");
    exit(2);
}

