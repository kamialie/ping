#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "ping.h"

long get_trip_time(struct timeval tv_begin) {
    long      time;
    struct timeval tv_end;

    if (gettimeofday(&tv_end, NULL) != 0) {
        perror("gettimeofday() error");
        exit(1);
    }
    time = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec);
    time += tv_end.tv_usec - tv_begin.tv_usec;
    return time;
}

void update_rt_stats(long time) {

    g_rt_stats.pkg_received += 1;
    if (time < g_rt_stats.min)
        g_rt_stats.min = time;
    if (time > g_rt_stats.max)
        g_rt_stats.max = time;
    g_rt_stats.sum += time;
    g_rt_stats.sum2 += time * time;
}