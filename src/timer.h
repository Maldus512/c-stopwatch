#ifndef C_STATE_TIMER_H_INCLUDED
#define C_STATE_TIMER_H_INCLUDED

#include <stdint.h>
#include "stopwatch.h"

typedef struct stopwatch_timer {
    stopwatch_t stopwatch;

    void (*callback)(struct stopwatch_timer *, void *, void *);
    void *arg;

    uint8_t fired : 1;
    uint8_t autoreload : 1;
} stopwatch_timer_t;

typedef void (*stopwatch_timer_callback_t)(stopwatch_timer_t *timer, void *user_ptr, void *arg);


int     stopwatch_timer_init(stopwatch_timer_t *timer, unsigned long period, stopwatch_timer_callback_t callback,
                             void *arg);
void    stopwatch_timer_set_autoreload(stopwatch_timer_t *timer, uint8_t autoreload);
int     stopwatch_timer_start(stopwatch_timer_t *timer, unsigned long timestamp);
int     stopwatch_timer_restart(stopwatch_timer_t *timer, unsigned long timestamp);
int     stopwatch_timer_pause(stopwatch_timer_t *timer, unsigned long timestamp);
void    stopwatch_timer_stop(stopwatch_timer_t *timer);
void    stopwatch_timer_trigger(stopwatch_timer_t *timer, void *user_ptr);
uint8_t stopwatch_timer_manage(stopwatch_timer_t *timer, unsigned long timestamp, void *user_ptr);

#endif