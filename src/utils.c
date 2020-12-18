#include <endian.h>
#include <stdlib.h>

u_int16_t	compute_checksum(u_int16_t *addr, int count)
{
	register u_int32_t sum;

	sum = 0;
	while (count > 1)
	{
		sum += *addr++;
		count -= 2;
	}
	if (count > 0)
		sum += *(unsigned char *)addr;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (~sum);
}

#if __BYTE_ORDER == __LITTLE_ENDIAN

u_int16_t	ft_htons(u_int16_t x)
{
	u_int16_t new;

	new = (x & 0xFF) << 8;
	new += (x & 0xFF00) >> 8;
	return (new);
}

#elif __BYTE_ORDER == __BIG_ENDIAN

u_int16_t	ft_htons(u_int16_t x)
{
	return (x);
}

#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN

u_int16_t	ft_ntohs(u_int16_t x)
{
	u_int16_t new;

	new = (x & 0xFF) << 8;
	new += (x & 0xFF00) >> 8;
	return (new);
}

#elif __BYTE_ORDER == __BIG_ENDIAN

u_int16_t	ft_ntohs(u_int16_t x)
{
	return (x);
}

#endif
