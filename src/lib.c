#include <lib.h>

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char *tmp;
	unsigned char value;

	value = (unsigned char)c;
	tmp = (unsigned char *)b;
	while (len-- > 0)
		*tmp++ = value;
	return (b);
}

char	*ft_strchr(const char *s, int c)
{
	char value;

	value = (char)c;
	while (*s != '\0')
	{
		if (value == *s)
			return ((char*)s);
		s++;
	}
	if (value == '\0')
		return ((char*)s);
	return (NULL);
}

int		ft_atoi(const char *str)
{
	size_t	i;
	int		sign;

	i = 0;
	sign = 1;
	while (*str == '\n' || *str == '\t' || *str == ' '
			|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '+' || *str == '-')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		i = i * 10 + (*str - '0');
		str++;
	}
	return (int)(i * sign);
}

size_t ft_strlen(const char *str)
{
	size_t len;

	len = 0;
	while (*str++ != '\0')
		len++;
	return (len);
}