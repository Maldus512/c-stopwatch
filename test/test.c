#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "stopwatch.h"


/* These functions will be used to initialize
   and clean resources up after each test run */
int setup(void **state) {
    (void)state;
    return 0;
}

int teardown(void **state) {
    (void)state;
    return 0;
}


static void stopwatch_init_test(void **state) {
    (void)state;

    stopwatch_t stopwatch;
    stopwatch_init(&stopwatch);

    assert_int_equal(STOPWATCH_STATE_STOPPED, stopwatch_get_state(&stopwatch));

    assert_int_equal(0, stopwatch_start(&stopwatch, 0));
    assert_int_equal(STOPWATCH_STATE_RUNNING, stopwatch_get_state(&stopwatch));

    assert_int_equal(-1, stopwatch_start(&stopwatch, 0));
}


static void stopwatch_count_test(void **state) {
    (void)state;

    stopwatch_t stopwatch;
    stopwatch_init(&stopwatch);

    stopwatch_start(&stopwatch, 0);
    assert_int_equal(0, stopwatch_get_elapsed(&stopwatch, 0));
    assert_int_equal(10, stopwatch_get_elapsed(&stopwatch, 10));
    assert_int_equal(1000, stopwatch_get_elapsed(&stopwatch, 1000));
    assert_int_equal(200, stopwatch_get_elapsed(&stopwatch, 200));

    stopwatch_stop(&stopwatch);
    assert_int_equal(STOPWATCH_STATE_STOPPED, stopwatch_get_state(&stopwatch));

    assert_int_equal(0, stopwatch_get_elapsed(&stopwatch, 0));
    assert_int_equal(0, stopwatch_get_elapsed(&stopwatch, 10));
    assert_int_equal(0, stopwatch_get_elapsed(&stopwatch, 1000));
}


static void stopwatch_set_test(void **state) {
    (void)state;

    stopwatch_t stopwatch;
    stopwatch_init(&stopwatch);

    assert_int_equal(0, stopwatch_set(&stopwatch, 1000));
    assert_int_equal(1000, stopwatch_get_total_time(&stopwatch));
    assert_int_equal(0, stopwatch_set(&stopwatch, 2000));
    assert_int_equal(2000, stopwatch_get_total_time(&stopwatch));

    assert_int_equal(STOPWATCH_STATE_STOPPED, stopwatch_get_state(&stopwatch));

    // Cannot set a started stopwatch; it has to be stopped first
    stopwatch_start(&stopwatch, 0);
    assert_int_equal(-1, stopwatch_set(&stopwatch, 2000));

    assert_int_equal(2000, stopwatch_get_remaining(&stopwatch, 0));
    assert_int_equal(1500, stopwatch_get_remaining(&stopwatch, 500));
    assert_false(stopwatch_is_done(&stopwatch, 500));
    assert_int_equal(500, stopwatch_get_remaining(&stopwatch, 1500));
    assert_int_equal(0, stopwatch_get_remaining(&stopwatch, 2000));
    assert_true(stopwatch_is_done(&stopwatch, 2001));
    assert_int_equal(0, stopwatch_get_remaining(&stopwatch, 2500));

    STOPWATCH_RESTART_WITH_NEW_PERIOD(&stopwatch, 42, 40);
    assert_false(stopwatch_is_done(&stopwatch, 40));
    assert_true(stopwatch_is_done(&stopwatch, 82));
    assert_int_equal(42, stopwatch_get_total_time(&stopwatch));
}


static void stopwatch_pause_test(void **state) {
    (void)state;

    stopwatch_t   stopwatch;
    unsigned long time = 0;
    stopwatch_init(&stopwatch);

    stopwatch_set(&stopwatch, 100);
    stopwatch_start(&stopwatch, time);
    time += 20;

    assert_int_equal(0, stopwatch_pause(&stopwatch, time));
    assert_int_equal(-1, stopwatch_pause(&stopwatch, time));

    time += 100;
    stopwatch_start(&stopwatch, time);

    assert_int_equal(80, stopwatch_get_remaining(&stopwatch, time));
    time += 70;
    assert_int_equal(10, stopwatch_get_remaining(&stopwatch, time));
    time += 10;
    assert_int_equal(0, stopwatch_get_remaining(&stopwatch, time));
}


void stopwatch_stress_test(void **state) {
    (void)state;

    unsigned long time = 0, elapsed = 0;
    stopwatch_t   stopwatch;
    stopwatch_init(&stopwatch);

    srand(42);

    stopwatch_set(&stopwatch, 100000000);
    stopwatch_start(&stopwatch, time);

    while (!stopwatch_is_done(&stopwatch, time)) {
        unsigned long delay = rand() % 1000;
        time += delay;

        if (stopwatch_get_state(&stopwatch) == STOPWATCH_STATE_RUNNING) {
            elapsed += delay;
        }

        switch (rand() % 2) {
            case 0:
                stopwatch_pause(&stopwatch, time);
                break;
            case 1:
                stopwatch_start(&stopwatch, time);
                break;
            default:
                break;
        }

        assert_int_equal(elapsed, stopwatch_get_elapsed(&stopwatch, time));
    }
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(stopwatch_init_test),   cmocka_unit_test(stopwatch_count_test),
        cmocka_unit_test(stopwatch_set_test),    cmocka_unit_test(stopwatch_pause_test),
        cmocka_unit_test(stopwatch_stress_test),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */
    int count_fail_tests = cmocka_run_group_tests(tests, setup, teardown);

    return count_fail_tests;
}
