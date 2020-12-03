#ifndef PING_H
# define PING_H

// TEMPRORARY
#include <stdio.h>
//END

typedef struct s_icmp_hdr {
    u_int8_t    type; // message type, 0 - echo reply, 8 - echo request
    u_int8_t    code; // used for error code
    u_int16_t   chksum; // checksum of header plus data
    u_int16_t   id; // identifier used to match requests/replies
    u_int16_t   seq; // used to identify the sequence if more than one is sent
}   t_icmp_hdr;

typedef struct s_icmp_data {
    u_int32_t   content;
}   t_icmp_data;

typedef struct s_icmp_pack {
    t_icmp_hdr  header;
    t_icmp_data data;
}   t_icmp_pack;

#endif
