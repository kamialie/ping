#include "ping.h"

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