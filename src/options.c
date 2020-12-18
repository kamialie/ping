#include <string.h>
#include "ping.h"
#include "lib.h"

void	options(int argv, char *args[], t_options *options)
{
	int		i;
	char	*str;

	i = 0;
	while (++i < argv)
	{
		str = args[i];
		if (*str++ != '-')
			return ;
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
	print_usage();
}

void	set_single_option(const char *str, t_options *options)
{
	char option;

	if (*(str + 1) != '\0')
		print_usage();
	option = *str;
	if (option == 'h')
		print_usage();
	else if (option == 'v')
		options->options |= V_FLAG;
}

void	set_options_with_arguments(char option, char *str, t_options *opt)
{
	if (option == 'p')
		set_p_option(str, opt);
	else
	{
		if (option == 't')
			opt->ttl = get_value(str, T_FLAG, 255, &opt->options);
		else if (option == 'c')
			opt->count = get_value(str, C_FLAG, 512, &opt->options);
		else if (option == 'l')
			opt->preload = get_value(str, L_FLAG, 10, &opt->options);
		else if (option == 's')
			opt->icmp_data_size = get_value(str, S_FLAG, 512, &opt->options);
	}
}

int		get_value(char *str, int option, int max, int *options)
{
	int value;

	value = ft_atoi(str);
	if (value <= 0 || value > max)
		exit_with_error(option);
	*options |= option;
	return (value);
}
