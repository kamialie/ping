#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "ping.h"

int	main(void)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == -1)
    {
        printf("error occurred while creating socket");
        exit(1);
    }
    if (setsockopt(sockfd, IPPROTO_IP, int optname, const void *optval, socklen_t optlen) != 0)
    {
        printf("error occurred while setting socket options");
        exit(1);
    }
	return (0);
}
