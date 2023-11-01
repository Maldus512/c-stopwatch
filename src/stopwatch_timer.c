#include "stopwatch_timer.h"


void stopwatch_timer_init(stopwatch_timer_t *timer, unsigned long period, stopwatch_timer_callback_t callback,
                          void *arg) {
    timer->callback = callback;
    timer->arg      = arg;

    timer->autoreload = 0;
    timer->fired      = 0;

    stopwatch_init(&timer->stopwatch);
    stopwatch_set(&timer->stopwatch, period);
}


void stopwatch_timer_set_period(stopwatch_timer_t *timer, unsigned long period) {
    stopwatch_set(&timer->stopwatch, period);
}


void stopwatch_timer_set_autoreload(stopwatch_timer_t *timer, uint8_t autoreload) {
    timer->autoreload = autoreload;
}


void stopwatch_timer_resume(stopwatch_timer_t *timer, unsigned long timestamp) {
    stopwatch_resume(&timer->stopwatch, timestamp);
}


void stopwatch_timer_reset(stopwatch_timer_t *timer, unsigned long timestamp) {
    timer->fired = 0;
    stopwatch_reset(&timer->stopwatch, timestamp);
}


void stopwatch_timer_pause(stopwatch_timer_t *timer, unsigned long timestamp) {
    stopwatch_pause(&timer->stopwatch, timestamp);
}


void stopwatch_timer_trigger(stopwatch_timer_t *timer, void *user_ptr) {
    timer->callback(timer, user_ptr);
}


uint8_t stopwatch_timer_manage(stopwatch_timer_t *timer, unsigned long timestamp, void *user_ptr) {
    if (stopwatch_is_done(&timer->stopwatch, timestamp) && !timer->fired) {
        timer->callback(timer, user_ptr);
        if (timer->autoreload) {
            timer->fired = 0;
            stopwatch_reset(&timer->stopwatch, timestamp);
        } else {
            timer->fired = 1;
        }
        return 1;
    } else {
        return 0;
    }
}


void *stopwatch_timer_get_arg(stopwatch_timer_t *timer) {
    return timer->arg;
}
