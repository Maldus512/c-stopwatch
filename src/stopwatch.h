#ifndef C_STOPWATCH_H_INCLUDED
#define C_STOPWATCH_H_INCLUDED


#include <stdint.h>


typedef struct {
    unsigned long starting_time;
    unsigned long elapsed_time;
    unsigned long total_time;
    uint8_t       paused;
} stopwatch_t;


void stopwatch_init(stopwatch_t *stopwatch);
void stopwatch_resume(stopwatch_t *stopwatch, unsigned long timestamp);
void stopwatch_pause(stopwatch_t *stopwatch, unsigned long timestamp);
void stopwatch_reset(stopwatch_t *stopwatch, unsigned long timestamp);
void stopwatch_set(stopwatch_t *stopwatch, unsigned long period);

unsigned long stopwatch_get_elapsed(const stopwatch_t *stopwatch, unsigned long timestamp);
void          stopwatch_set_elapsed(stopwatch_t *stopwatch, unsigned long elapsed);
unsigned long stopwatch_get_remaining(const stopwatch_t *stopwatch, unsigned long timestamp);
unsigned long stopwatch_get_total_time(const stopwatch_t *stopwatch);
uint8_t       stopwatch_is_done(const stopwatch_t *stopwatch, unsigned long timestamp);
uint8_t       stopwatch_is_paused(const stopwatch_t *stopwatch);
unsigned long stopwatch_get_overtime(const stopwatch_t *stopwatch, unsigned long timestamp);

#endif
