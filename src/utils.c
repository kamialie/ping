#include "ping.h"

#include <arpa/inet.h>

void    print_memory(void *memory, unsigned int len) {
    unsigned int  i;
    unsigned char *buf;

    i = 0;
    buf = (unsigned char *)memory;
    printf("0x0000: ");
    while (i < len) {
        printf("%02x", buf[i]);
        i++;
        if (i % 16 == 0)
            printf("\n%0#6x: ", i);
        else if (i % 2 == 0)
            printf(" ");
    }
    printf("\n");
}

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