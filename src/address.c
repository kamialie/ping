#include <stdio.h>
#include <netinet/in.h> // protocol macros

#include <sys/socket.h>
#include <netdb.h>

#include "ping.h"
#include "lib.h"

void ft_freeaddrinfo(struct addrinfo *addrinfo);

struct sockaddr_in get_address(char *input)
{
	int				status;
	struct addrinfo	hints;
	struct addrinfo	*res;
	struct addrinfo	*head;
	struct sockaddr_in addr;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo(input, 0, &hints, &res)) < 0)
	{
		if (status == -2)
			fprintf(stderr, "ft_ping: %s: Name or service not known\n", input);
		else
			fprintf(stderr, "ft_ping: getaddrinfo() error\n");
		exit(2);
	}
	head = res;
    while (res != NULL)
    {
        if (res->ai_family == AF_INET)
		{
        	addr = *(struct sockaddr_in *)res->ai_addr;
			ft_freeaddrinfo(head);
			return addr;
		}
		res = res->ai_next;
    }
	fprintf(stderr, "ft_ping: getaddrinfo() suitable address not found\n");
    exit(2);
}

void ft_freeaddrinfo(struct addrinfo *addrinfo)
{
	struct addrinfo *current;

	while (addrinfo != NULL)
	{
		current = addrinfo;
		addrinfo = addrinfo->ai_next;
		free(current->ai_canonname);
		free(current);
	}
}