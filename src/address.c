#include <stdio.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <netdb.h>

#include "ping.h"
#include "lib.h"

void	get_address(char *input, t_info *info)
{
	int				status;
	struct addrinfo	hints;
	struct addrinfo	*res;

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
	extract_address(res, info);
}

void	extract_address(struct addrinfo *head, t_info *info)
{
	struct addrinfo	*tmp;

	tmp = head;
	while (head != NULL)
	{
		if (head->ai_family == AF_INET)
		{
			info->address_info = *(struct sockaddr_in *)head->ai_addr;
			ft_freeaddrinfo(tmp);
			return ;
		}
		head = head->ai_next;
	}
	fprintf(stderr, "ft_ping: getaddrinfo() suitable address not found\n");
	exit(2);
}

void	ft_freeaddrinfo(struct addrinfo *addrinfo)
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
