#include <stdio.h>
#include <netinet/in.h> // protocol macros

#include <sys/socket.h>
#include <netdb.h>

#include "ping.h"
#include "lib.h"

struct sockaddr_in get_address(char *input)
{
	struct addrinfo hints, *res;
	int status;

	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
	hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo(input, 0, &hints, &res)) < 0)
	{
		if (status == -2)
			fprintf(stderr, "ft_ping: %s: Name or service not known\n", input);
		else
			fprintf(stderr, "ft_ping: getaddrinfo() error\n");
		exit(2);
	}
    while (res != NULL)
    {
        if (res->ai_family == AF_INET)
		{
//        	freeaddrinfo()
			return *(struct sockaddr_in *)res->ai_addr;
		}
        res++;
    }
	fprintf(stderr, "ft_ping: getaddrinfo() suitable address not found\n");
    exit(2);
}

//TODO add freeaddrinfo (will do while checking with valgrind)