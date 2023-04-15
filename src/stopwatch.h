#ifndef C_STATE_STOPWATCH_H_INCLUDED
#define C_STATE_STOPWATCH_H_INCLUDED


#include <stdint.h>


#define STOPWATCH_RESTART_WITH_NEW_PERIOD(s, period, timestamp)                                                        \
    {                                                                                                                  \
        stopwatch_stop(s);                                                                                     \
        stopwatch_set(s, period);                                                                                      \
        stopwatch_start(s, timestamp);                                                                                 \
    }

#define STOPWATCH_CONTINUE_WITH_NEW_PERIOD(s, period, timestamp)                                                       \
    {                                                                                                                  \
        stopwatch_set(s, period);                                                                                      \
        stopwatch_start(s, timestamp);                                                                                 \
    }


typedef enum {
    STOPWATCH_STATE_STOPPED = 0,
    STOPWATCH_STATE_PAUSED,
    STOPWATCH_STATE_RUNNING,
} stopwatch_state_t;


typedef struct {
    unsigned long starting_time;
    unsigned long elapsed_time;
    unsigned long total_time;

    stopwatch_state_t state;
} stopwatch_t;


void stopwatch_init(stopwatch_t *stopwatch);
int  stopwatch_start(stopwatch_t *stopwatch, unsigned long timestamp);
int  stopwatch_pause(stopwatch_t *stopwatch, unsigned long timestamp);
int  stopwatch_restart(stopwatch_t *stopwatch, unsigned long timestamp);
int  stopwatch_set(stopwatch_t *stopwatch, unsigned long period);
void stopwatch_stop(stopwatch_t *stopwatch);

stopwatch_state_t stopwatch_get_state(stopwatch_t *stopwatch);
unsigned long     stopwatch_get_elapsed(stopwatch_t *stopwatch, unsigned long timestamp);
void              stopwatch_set_elapsed(stopwatch_t *stopwatch, unsigned long elapsed);
unsigned long     stopwatch_get_remaining(stopwatch_t *stopwatch, unsigned long timestamp);
unsigned long     stopwatch_get_total_time(stopwatch_t *stopwatch);
uint8_t           stopwatch_is_done(stopwatch_t *stopwatch, unsigned long timestamp);

#endif