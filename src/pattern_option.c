#include <stdlib.h>

#include "lib.h"
#include "ping.h"

static int		char_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	else
		exit_with_error(PATTERN_ERROR);
	return (-1);
}

void			set_p_option(char *str, t_options *options)
{
	size_t			len;
	size_t			i;
	size_t			j;
	unsigned char	*pattern;

	options->options |= P_FLAG;
	if ((len = ft_strlen(str)) > 16)
		exit_with_error(PATTERN_ERROR);
	options->patternlen = (int)len;
	pattern = (unsigned char *)&options->pattern;
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i + 1] != '\0')
		{
			pattern[j++] = (unsigned char)((char_to_int(str[i]) << 4) +
											char_to_int(str[i + 1]));
		}
		else
			pattern[j++] = (unsigned char)(char_to_int(str[i]));
		i += 2;
	}
}
