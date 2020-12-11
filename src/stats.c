#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "ping.h"

long get_trip_time(struct timeval tv_begin) {
    long      time;
    struct timeval tv_end;

    if (gettimeofday(&tv_end, NULL) != 0)
		exit_with_error(GETTIMEOFDAY_ERROR);
    time = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec);
    time += tv_end.tv_usec - tv_begin.tv_usec;
    return time;
}

void update_rt_stats(long time) {
    t_rt_stats *rt_stats;

    rt_stats = g_info.rt_stats;
    if (time < rt_stats->min)
        rt_stats->min = time;
    if (time > rt_stats->max)
        rt_stats->max = time;
    rt_stats->sum += time;
    rt_stats->sum2 += time * time;
}