#include <string.h>
#include <stdio.h>
#include "ping.h"
#include "lib.h"

void set_single_option(char flag);
void set_options_with_arguments(char flag, char *str);

int options(int argv, char *args[])
{
    int i;
    char *str;
    char *single_options = "bhv";
    char *options_with_arg = "ct";

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
                set_single_option(*str);
            }
            else if (ft_strchr(options_with_arg, *str) != NULL)
            {
                if (*(str + 1) != '\0')
                    set_options_with_arguments(*str, str + 1);
                else
                {
                    printf("one - %d, two - %d\n", i + 1, argv - 1);
                    if (i + 2 > argv - 1)
                        print_usage();
                    set_options_with_arguments(*str, args[++i]);
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

void set_single_option(char flag)
{
    printf("flag - %c\n", flag);
    if (flag == 'h')
        g_info.options |= H_FLAG;
}

void set_options_with_arguments(char flag, char *str)
{
    int value;

    printf("str - %s\n", str);
    printf("flag with options\n");
    value = ft_atoi(str);
    printf("value - %d\n", value);
    if (value < 0 || value > 100000)
        printf("6\n");//print_usage();
    if (flag == 't')
    {
        g_info.options |= T_FLAG;
        g_info.ttl = value;
    }
}
