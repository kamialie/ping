#include <endian.h>
#include "ping.h"
#include "lib.h"

u_int16_t compute_checksum(u_int16_t *addr, int count)
{
    register u_int32_t  sum = 0;

    while (count > 1) {
        sum += *addr++;
        count -= 2;
    }
    if( count > 0 )
        sum += * (unsigned char *) addr;
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);
    return (~sum);
}

u_int16_t ft_htons (u_int16_t x)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t new;

    new = (x & 0xFF) << 8;
    new += (x & 0xFF << 8) >> 8;
    return new;
#elif __BYTE_ORDER == __BIG_ENDIAN
    return x;
#endif
}

int get_value(const char *str)
{
    int value;

    value = ft_atoi(str);
    if (value < 0 || value > 100000)
        print_usage();
    return value;
}