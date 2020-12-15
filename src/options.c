#include <string.h>
#include <stdio.h>
#include "ping.h"
#include "lib.h"

void set_single_option(char option, t_options *options);
void set_options_with_arguments(char option, char *str, t_options *options);
void handle_p_option(char *str, t_options *options);

int options(int argv, char *args[], t_options *options)
{
    int i;
    char *str;
    char *single_options = "hv";
    char *options_with_arg = "cpt";

//    if (argv < 2)
//        print_usage();
//    printf("argv - %d i - %d\n", argv, i);
    i = 1;
    while (i < argv)
    {
        str = args[i];
        if (*str++ == '-')
        {
            if (ft_strchr(single_options, *str) != NULL)
            {
                if (*(str + 1) != '\0')
                    printf("2\n");//print_usage();
                set_single_option(*str, options);
            }
            else if (ft_strchr(options_with_arg, *str) != NULL)
            {
                if (*(str + 1) != '\0')
                    set_options_with_arguments(*str, str + 1, options);
                else
                {
                    if (i + 2 > argv - 1)
                        print_usage();
                    set_options_with_arguments(*str, args[++i], options);
                }
            }
            else
                printf("3\n");//print_usage();
        }
        else
            return 1;
        i++;
    }
    printf("5\n");//print_usage();
    return 1;
}

void set_single_option(char option, t_options *options)
{
    if (option == 'h')
        options->options |= H_FLAG;
}

void set_options_with_arguments(char option, char *str, t_options *options)
{
    int value;

    if (option == 't')
    {
		value = ft_atoi(str);
		if (value <= 0 || value > 255)
			exit_with_error(TTL_OPTION_ERROR);
		options->options |= T_FLAG;
		options->ttl = value;
    }
    else if (option == 'c')
	{
		value = ft_atoi(str);
		if (value <= 0)
			exit_with_error(COUNT_OPTION_ERROR);
		options->options |= C_FLAG;
		options->count = value;
	}
    else if (option == 'p')
	{
		options->options |= P_FLAG;
		handle_p_option(str, options);
	}
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

void handle_p_option(char *str, t_options *options)
{
	size_t 	len;
	size_t 	i;
	size_t 	j;
	unsigned char 	*pattern;

	if ((len = ft_strlen(str)) > 16)
		exit_with_error(PATTERN_ERROR);
	options->patternlen = (int) len;
	pattern = (unsigned char *)&options->pattern;
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i + 1] != '\0')
			pattern[j++] = (unsigned char) ((char_to_int(str[i]) << 4) + char_to_int(str[i + 1]));
		else
			pattern[j++] = (unsigned char) (char_to_int(str[i]));
		i += 2;
	}
}
