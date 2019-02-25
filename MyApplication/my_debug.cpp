#include "my_debug.h"


#define HEX_DUMP_NUM_IN_LINE 8   //8, 16


void hex_dump(const char *desc, void *addr, uint32_t len) {
    uint32_t i;
    uint8_t buff[HEX_DUMP_NUM_IN_LINE+1];     // stores the ASCII data
    uint8_t *p = (uint8_t *)addr;     // cast to make the code cleaner.

    // Output description if given.
    if(desc != NULL)
        DEBUG_OUT("%s:\n", desc);

    // Process every byte in the data.
    for(i = 0; i < len; i++){
        // Multiple of 16 means new line (with line offset).
        if((i % HEX_DUMP_NUM_IN_LINE) == 0) {
            // Just don't print ASCII for the zeroth line.
            if(i != 0)
                DEBUG_OUT("  %s\n", buff);

            // Output the offset.
            DEBUG_OUT("  %04X ", i);
        }

        // Now the hex code for the specific character.
        DEBUG_OUT(" %02X", p[i]);

        // And store a printable ASCII character for later.
        if((p[i] < 0x20) || (p[i] > 0x7e))
            buff[i % HEX_DUMP_NUM_IN_LINE] = '.';
        else
            buff[i % HEX_DUMP_NUM_IN_LINE] = p[i];
        buff[(i % HEX_DUMP_NUM_IN_LINE) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % HEX_DUMP_NUM_IN_LINE) != 0) {
        DEBUG_OUT("   ");
        i++;
    }

    // And print the final ASCII bit.
    DEBUG_OUT("  %s\n", buff);
}

/* NOTE: Require adding the command-line flag -DMBED_HEAP_STATS_ENABLED=1 */
void heap_statistics(bool detail) {
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);

    if (detail) {
        DEBUG_OUT("current_size: %lu\n", heap_stats.current_size);
        DEBUG_OUT("max_size: %lu\n", heap_stats.max_size);
        DEBUG_OUT("total_size: %lu\n", heap_stats.total_size);
        DEBUG_OUT("reserved_size: %lu\n", heap_stats.reserved_size);
        DEBUG_OUT("alloc_cnt: %lu\n", heap_stats.alloc_cnt);
        DEBUG_OUT("alloc_fail_cnt: %lu\n", heap_stats.alloc_fail_cnt);
    } else {
        DEBUG_OUT("Current heap: %lu\n", heap_stats.current_size);
        DEBUG_OUT("Max heap size: %lu\n", heap_stats.max_size);
    }
    DEBUG_OUT("\n");
}

/* NOTE: Require adding the command-line flag -DMBED_STACK_STATS_ENABLED=1 */
void stack_statistics(bool detail) {
    int cnt = osThreadGetCount();
    mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));
    cnt = mbed_stats_stack_get_each(stats, cnt);

    for (int i = 0; i < cnt; i++) {
        if (detail) {
            DEBUG_OUT("thread_id: 0x%lX\n", stats[i].thread_id);
            DEBUG_OUT("max_size: %lu\n", stats[i].max_size);
            DEBUG_OUT("reserved_size: %lu\n", stats[i].reserved_size);
            DEBUG_OUT("stack_cnt: %lu\n", stats[i].stack_cnt);
        } else {
            DEBUG_OUT("Thread: 0x%lX, Stack size: %lu, Max stack: %lu\n", stats[i].thread_id, stats[i].reserved_size, stats[i].max_size);
        }
    }
    DEBUG_OUT("\n");
}


