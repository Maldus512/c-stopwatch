#include "timer.h"


int stopwatch_timer_init(stopwatch_timer_t *timer, unsigned long period, stopwatch_timer_callback_t callback,
                         void *arg) {
    timer->callback = callback;
    timer->arg      = arg;

    timer->autoreload = 0;
    timer->fired      = 0;

    stopwatch_init(&timer->stopwatch);
    return stopwatch_set(&timer->stopwatch, period);
}


void stopwatch_timer_set_autoreload(stopwatch_timer_t *timer, uint8_t autoreload) {
    timer->autoreload = autoreload;
}


int stopwatch_timer_start(stopwatch_timer_t *timer, unsigned long timestamp) {
    return stopwatch_start(&timer->stopwatch, timestamp);
}


int stopwatch_timer_restart(stopwatch_timer_t *timer, unsigned long timestamp) {
    timer->fired = 0;
    return stopwatch_restart(&timer->stopwatch, timestamp);
}


int stopwatch_timer_pause(stopwatch_timer_t *timer, unsigned long timestamp) {
    return stopwatch_pause(&timer->stopwatch, timestamp);
}


void stopwatch_timer_stop(stopwatch_timer_t *timer) {
    stopwatch_stop(&timer->stopwatch);
}


void stopwatch_timer_trigger(stopwatch_timer_t *timer, void *user_ptr) {
    timer->callback(timer, user_ptr, timer->arg);
}


uint8_t stopwatch_timer_manage(stopwatch_timer_t *timer, unsigned long timestamp, void *user_ptr) {
    if (stopwatch_is_done(&timer->stopwatch, timestamp) && !timer->fired) {
        timer->callback(timer, user_ptr, timer->arg);
        if (timer->autoreload) {
            timer->fired = 0;
            stopwatch_restart(&timer->stopwatch, timestamp);
        } else {
            timer->fired = 1;
        }
        return 1;
    } else {
        return 0;
    }
}