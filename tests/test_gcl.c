#include <assert.h>
#include <stdio.h>
#include "tsn_gcl.h"
int main(void) {
    tsn_gcl_t gcl; tsn_gcl_init(&gcl);
    assert(tsn_gcl_add(&gcl,0x01,100)==0);
    assert(tsn_gcl_add(&gcl,0x02,200)==0);
    assert(tsn_gcl_add(&gcl,0x04,100)==0);
    assert(tsn_gcl_finalize(&gcl)==0);
    assert(gcl.cycle_time_us==400);
    assert(tsn_gcl_gate_at(&gcl,50)==0x01);
    assert(tsn_gcl_gate_at(&gcl,150)==0x02);
    assert(tsn_gcl_gate_at(&gcl,299)==0x02);
    assert(tsn_gcl_gate_at(&gcl,350)==0x04);
    assert(tsn_gcl_gate_at(&gcl,450)==0x01);
    puts("GCL tests passed."); return 0;
}
