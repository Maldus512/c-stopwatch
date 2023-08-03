#ifndef C_STOPWATCH_TIMER_H_INCLUDED
#define C_STOPWATCH_TIMER_H_INCLUDED

#include <stdint.h>
#include "stopwatch.h"

typedef struct stopwatch_timer {
    stopwatch_t stopwatch;

    void (*callback)(struct stopwatch_timer *, void *);
    void *arg;

    uint8_t fired : 1;
    uint8_t autoreload : 1;
} stopwatch_timer_t;

typedef void (*stopwatch_timer_callback_t)(stopwatch_timer_t *timer, void *user_ptr);


void    stopwatch_timer_init(stopwatch_timer_t *timer, unsigned long period, stopwatch_timer_callback_t callback,
                             void *arg);
void    stopwatch_timer_set_autoreload(stopwatch_timer_t *timer, uint8_t autoreload);
void    stopwatch_timer_set_period(stopwatch_timer_t *timer, unsigned long period);
void    stopwatch_timer_resume(stopwatch_timer_t *timer, unsigned long timestamp);
void    stopwatch_timer_reset(stopwatch_timer_t *timer, unsigned long timestamp);
void    stopwatch_timer_pause(stopwatch_timer_t *timer, unsigned long timestamp);
void    stopwatch_timer_trigger(stopwatch_timer_t *timer, void *user_ptr);
uint8_t stopwatch_timer_manage(stopwatch_timer_t *timer, unsigned long timestamp, void *user_ptr);
void   *stopwatch_timer_get_arg(stopwatch_timer_t *timer);

#endif
