#include "stopwatch.h"
#include "_timecheck.h"


void stopwatch_init(stopwatch_t *stopwatch) {
    stopwatch->starting_time = 0;
    stopwatch->total_time    = 0;
    stopwatch->elapsed_time  = 0;
    stopwatch->state         = STOPWATCH_STATE_STOPPED;
}


int stopwatch_start(stopwatch_t *stopwatch, unsigned long timestamp) {
    switch (stopwatch->state) {
        case STOPWATCH_STATE_STOPPED:
        case STOPWATCH_STATE_PAUSED:
            stopwatch->starting_time = timestamp;
            stopwatch->state         = STOPWATCH_STATE_RUNNING;
            return 0;
        case STOPWATCH_STATE_RUNNING:     // Cannot start a counting stopwatch
            return -1;
    }
    return -1;
}


void stopwatch_stop(stopwatch_t *stopwatch) {
    stopwatch->starting_time = 0;
    stopwatch->elapsed_time  = 0;
    stopwatch->state         = STOPWATCH_STATE_STOPPED;
}


int stopwatch_pause(stopwatch_t *stopwatch, unsigned long timestamp) {
    switch (stopwatch->state) {
        case STOPWATCH_STATE_RUNNING:
            stopwatch->state = STOPWATCH_STATE_PAUSED;
            stopwatch->elapsed_time += time_interval(stopwatch->starting_time, timestamp);
            return 0;
        case STOPWATCH_STATE_STOPPED:     // Cannot pause a stopped stopwatch
        case STOPWATCH_STATE_PAUSED:      // Cannot pause a paused stopwatch
            return -1;
    }
    return -1;
}


int stopwatch_restart(stopwatch_t *stopwatch, unsigned long timestamp) {
    stopwatch_stop(stopwatch);
    return stopwatch_start(stopwatch, timestamp);
}


int stopwatch_set(stopwatch_t *stopwatch, unsigned long period) {
    if (stopwatch->state != STOPWATCH_STATE_STOPPED) {     // Can only set a stopped stopwatch
        return -1;
    } else {
        stopwatch->total_time = period;
        return 0;
    }
}


stopwatch_state_t stopwatch_get_state(stopwatch_t *stopwatch) {
    return stopwatch->state;
}


unsigned long stopwatch_get_elapsed(stopwatch_t *stopwatch, unsigned long timestamp) {
    if (stopwatch->state == STOPWATCH_STATE_RUNNING) {
        return stopwatch->elapsed_time + time_interval(stopwatch->starting_time, timestamp);
    } else {
        return stopwatch->elapsed_time;
    }
}


void stopwatch_set_elapsed(stopwatch_t *stopwatch, unsigned long elapsed) {
    stopwatch->elapsed_time = elapsed;
}


unsigned long stopwatch_get_remaining(stopwatch_t *stopwatch, unsigned long timestamp) {
    unsigned long elapsed = stopwatch_get_elapsed(stopwatch, timestamp);
    return elapsed < stopwatch->total_time ? stopwatch->total_time - elapsed : 0;
}


unsigned long stopwatch_get_total_time(stopwatch_t *stopwatch) {
    return stopwatch->total_time;
}


uint8_t stopwatch_is_done(stopwatch_t *stopwatch, unsigned long timestamp) {
    switch (stopwatch->state) {
        case STOPWATCH_STATE_STOPPED:
            return 0;

        case STOPWATCH_STATE_RUNNING: {
            if (stopwatch->total_time > stopwatch->elapsed_time) {
                unsigned long timeleft = stopwatch->total_time - stopwatch->elapsed_time;
                return is_expired(stopwatch->starting_time, timestamp, timeleft);
            } else {
                return 1;
            }
        }

        case STOPWATCH_STATE_PAUSED:
            return stopwatch->elapsed_time >= stopwatch->total_time;
    }
    return 0;
}
