#include <assert.h>
#include <stdio.h>
#include "tsn_clock.h"
#include "tsn_scheduler.h"
#include "tsn_traffic.h"
#include "tsn_monitor.h"
#include "tsn_node.h"

static void test_clock(void) {
    tsn_clock_t c;
    tsn_clock_init(&c, 7);
    tsn_clock_tick(&c, 1000000);
    assert(tsn_clock_now(&c) == 1000000);
    assert(tsn_clock_apply_sync(&c, 2500) == TSN_OK);
    assert(tsn_clock_now(&c) == 1002500);
}

static void test_scheduler(void) {
    tsn_scheduler_t s;
    tsn_scheduler_init(&s);
    tsn_schedule_entry_t e = { .stream_id=0x10, .period_us=1000, .phase_us=100,
                               .deadline_us=100, .max_jitter_us=20 };
    assert(tsn_scheduler_add(&s, e) == TSN_OK);
    assert(tsn_scheduler_find(&s, 0x10) != NULL);
    assert(tsn_scheduler_next_release_us(&e, 1250) == 2100);
    assert(tsn_scheduler_next_release_us(&e, 2050) == 2100);
    assert(tsn_scheduler_is_released(&e, 1100));
}

static void test_frame(void) {
    tsn_frame_t f;
    uint8_t data[] = {1,2,3};
    assert(tsn_frame_build(&f, 1, 4, 1000, data, 3) == TSN_OK);
    assert(tsn_frame_validate(&f) == TSN_OK);
    tsn_frame_deliver(&f, 1800);
    assert(f.rx_time_ns == 1800);
    assert(tsn_frame_build(&f, 2, 5, 0, NULL, TSN_MAX_PAYLOAD + 1u) == TSN_EINVAL);
}

static void test_monitor(void) {
    tsn_monitor_t m;
    tsn_monitor_init(&m);
    tsn_latency_sample_t a = tsn_measure_latency(1000000, 1010000, 1060000, 100, 20);
    tsn_latency_sample_t b = tsn_measure_latency(2000000, 2030000, 2135000, 100, 20);
    tsn_monitor_record(&m, &a);
    tsn_monitor_record(&m, &b);
    assert(a.within_deadline);
    assert(a.within_jitter);
    assert(!b.within_jitter);
    assert(!b.within_deadline);
    assert(m.samples == 2);
    assert(m.max_latency_ns == 105000);
    assert(tsn_monitor_average_latency_ns(&m) == 77500);
}

static void test_node(void) {
    tsn_node_t n;
    tsn_node_init(&n, 1);
    tsn_schedule_entry_t e = { .stream_id=0x22, .period_us=1000, .phase_us=0,
                               .deadline_us=100, .max_jitter_us=20 };
    assert(tsn_scheduler_add(&n.scheduler, e) == TSN_OK);
    assert(tsn_node_sync(&n, 0) == TSN_OK);
    assert(tsn_node_run_control_cycle(&n, 1000000, 0x22, 1000000, 1050000) == TSN_OK);
}

int main(void) {
    test_clock();
    test_scheduler();
    test_frame();
    test_monitor();
    test_node();
    puts("All TSN tests passed.");
    return 0;
}
