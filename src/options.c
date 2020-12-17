#include <string.h>
#include "ping.h"
#include "lib.h"

void set_single_option(const char *str, t_options *options);
void set_options_with_arguments(char option, char *str, t_options *options);
void set_p_option(char *str, t_options *options);
int	get_value(char *str, int option, int min, int max, int *options);

void options(int argv, char *args[], t_options *options)
{
	int i;
	char *str;

	i = 0;
	while (++i < argv)
	{
		str = args[i];
		if (*str++ == '-')
		{
			if (ft_strchr(SINGLE_OPTIONS, *str) != NULL)
				set_single_option(str, options);
			else if (ft_strchr(OPTIONS_WITH_ARGUMENTS, *str) != NULL)
			{
				if (*(str + 1) != '\0')
					set_options_with_arguments(*str, str + 1, options);
				else if (i + 2 > argv - 1)
					print_usage();
				else
					set_options_with_arguments(*str, args[++i], options);
			}
			else
				print_usage();
		}
		else
			return ;
	}
	print_usage();
}

void set_single_option(const char *str, t_options *options)
{
	char option;

	if (*(str + 1) != '\0')
		print_usage();
	option = *str;
	if (option == 'h')
		options->options |= H_FLAG;
	else if (option == 'v')
		options->options |= V_FLAG;
}

void set_options_with_arguments(char option, char *str, t_options *opt)
{
	if (option == 'p')
		set_p_option(str, opt);
	else
	{
		if (option == 't')
			opt->ttl = get_value(str, T_FLAG, 0, 255, &opt->options);
		else if (option == 'c')
			opt->count = get_value(str, C_FLAG, 0, 512, &opt->options);
		else if (option == 'l')
			opt->preload = get_value(str, L_FLAG, 0, 10, &opt->options);
		else if (option == 's')
			opt->icmp_data_size = get_value(str, S_FLAG, 0, 512, &opt->options);
	}
}

int	get_value(char *str, int option, int min, int max, int *options)
{
	int value;

	value = ft_atoi(str);
	if (value <= min || value > max)
		exit_with_error(option);
	*options |= option;
	return (value);
}

static int char_to_int(char c)
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

void set_p_option(char *str, t_options *options)
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
