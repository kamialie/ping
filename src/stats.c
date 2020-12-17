#include <sys/time.h>
#include <stdio.h>

#include "ping.h"

long get_trip_time(struct timeval *tv_begin)
{
    long      time;
    struct timeval tv_end;

    if (gettimeofday(&tv_end, NULL) != 0)
		exit_with_error(GETTIMEOFDAY_ERROR);
    time = 1000000 * (tv_end.tv_sec - tv_begin->tv_sec);
    time += tv_end.tv_usec - tv_begin->tv_usec;
    return time;
}

double update_rt_stats(struct timeval *tv_in, t_rt_stats *stats)
{
	double      time_milli_s;
	long        time_micro_s;

	time_micro_s = get_trip_time(tv_in);
	time_milli_s = (double) time_micro_s / 1000.0;
	if (time_micro_s < stats->min)
        stats->min = time_micro_s;
    if (time_micro_s > stats->max)
        stats->max = time_micro_s;
    stats->sum += time_micro_s;
    return (time_milli_s);
}