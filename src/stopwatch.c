#include "stopwatch.h"
#include "_timecheck.h"


void stopwatch_init(stopwatch_t *stopwatch) {
    stopwatch->starting_time = 0;
    stopwatch->total_time    = 0;
    stopwatch->elapsed_time  = 0;
    stopwatch->paused        = 1;
}


void stopwatch_resume(stopwatch_t *stopwatch, unsigned long timestamp) {
    if (stopwatch->paused) {
        stopwatch->starting_time = timestamp;
        stopwatch->paused        = 0;
    }
}


void stopwatch_pause(stopwatch_t *stopwatch, unsigned long timestamp) {
    if (!stopwatch->paused) {
        stopwatch->paused = 1;
        stopwatch->elapsed_time += time_interval(stopwatch->starting_time, timestamp);
    }
}


void stopwatch_reset(stopwatch_t *stopwatch, unsigned long timestamp) {
    stopwatch->elapsed_time  = 0;
    stopwatch->starting_time = timestamp;
}


void stopwatch_set(stopwatch_t *stopwatch, unsigned long period) {
    stopwatch->total_time = period;
}


uint8_t stopwatch_is_paused(const stopwatch_t *stopwatch) {
    return stopwatch->paused;
}


unsigned long stopwatch_get_elapsed(const stopwatch_t *stopwatch, unsigned long timestamp) {
    if (!stopwatch->paused) {
        return stopwatch->elapsed_time + time_interval(stopwatch->starting_time, timestamp);
    } else {
        return stopwatch->elapsed_time;
    }
}


void stopwatch_set_elapsed(stopwatch_t *stopwatch, unsigned long elapsed) {
    stopwatch->elapsed_time = elapsed;
}


unsigned long stopwatch_get_remaining(const stopwatch_t *stopwatch, unsigned long timestamp) {
    unsigned long elapsed = stopwatch_get_elapsed(stopwatch, timestamp);
    return elapsed < stopwatch->total_time ? stopwatch->total_time - elapsed : 0;
}


unsigned long stopwatch_get_overtime(const stopwatch_t *stopwatch, unsigned long timestamp) {
    long overtime = stopwatch_get_elapsed(stopwatch, timestamp) - stopwatch_get_total_time(stopwatch);
    return overtime > 0 ? overtime : 0;
}


unsigned long stopwatch_get_total_time(const stopwatch_t *stopwatch) {
    return stopwatch->total_time;
}


uint8_t stopwatch_is_done(const stopwatch_t *stopwatch, unsigned long timestamp) {
    if (stopwatch->paused) {
        return stopwatch->elapsed_time >= stopwatch->total_time;
    } else {
        if (stopwatch->total_time > stopwatch->elapsed_time) {
            unsigned long timeleft = stopwatch->total_time - stopwatch->elapsed_time;
            return is_expired(stopwatch->starting_time, timestamp, timeleft);
        } else {
            return 1;
        }
    }
}
