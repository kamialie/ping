#include "ping.h"

#include <arpa/inet.h>

u_int16_t compute_checksum(u_int16_t *addr, int count) {
    register u_int32_t  sum = 0;

    while (count > 1) {
        sum += *addr++;
        count -= 2;
    }
    if( count > 0 )
        sum += * (unsigned char *) addr;
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);
    return (htons(~sum));
}

// TODO htons ntohs memset