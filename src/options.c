#include "ping.h"
#include "lib.h"
#include <string.h>

void set_single_option(char flag, t_info *info);
void set_options_with_arguments(char flag, char *str, t_info *info);

int options(int argv, char *args[], t_info *info)
{
    int i;
    char *str;
    char *single_options = "hv";
    char *options_with_arg = "ct";

//    if (argv < 2)
//        print_usage();
    printf("argv - %d i - %d\n", argv, i);
    i = 1;
    while (i < argv)
    {
        printf("hey\n");
        str = args[i];
        if (*str++ == '-')
        {
            if (strlen(str) == 1)
            {
                if (ft_strchr(single_options, *str) != NULL)
                    set_single_option(*str, info);
                else if (ft_strchr(options_with_arg, *str) != NULL)
                {
                    if (*(str + 1) != '\0')
                        set_options_with_arguments(*str, str + 1, info);
                    else
                    {
                        printf("one - %d, two - %d\n", i + 1, argv - 1);
                        if (i + 2 > argv - 1)
                            print_usage();
                        set_options_with_arguments(*str, args[++i], info);
                    }
                }
                else
                    printf("3\n");//print_usage();
            }
            else
                printf("4\n");//print_usage();
        }
        else
            return 1;
        i++;
    }
    printf("5\n");//print_usage();
    return 1;
}

void set_single_option(char flag, t_info *info)
{
    printf("flag - %c\n", flag);
    if (flag == 'h')
        info->flags |= H_FLAG;
}

void set_options_with_arguments(char flag, char *str, t_info *info)
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
        if (value == 0) // maybe ttl set will error this
            printf("7\n");//print_usage();
        info->ttl = value;
    }
}
