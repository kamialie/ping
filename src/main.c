#include "ping.h"
#include <unistd.h>

int	main()
{
    t_info info;

    info.sfd = get_socket();
    fill_icmp_packet(&info.icmp_packet);
    prepare_destination(&info.sin);
    send_packet(info.sfd, &info.icmp_packet, &info.sin);
    receive_packet(info.sfd);

    close(info.sfd);

    return (0);
}