#ifndef MY_DEBUG_H
#define MY_DEBUG_H

#include "mbed.h"
#include "mbed_stats.h"

#if     (MBED_CONF_APP_RUN_MODE == 0)

    /* Disable serial debug */
    #define DEBUG_SERIAL(...)               // Empty
    #define DEBUG_OUT(...)                  do{}while(0)
    #define DEBUG_IN(...)                   do{}while(0)
    #define PRESS(c)                        do{}while(0)
    #define PRESS_ANY_KEY_TO(s)             do{}while(0)
    #define PRESS_ANY_KEY_TO_BREAK_LOOP()   do{}while(0)
    #define ASSERT(expr)                    do{}while(0)
    #define HEX_DUMP(desc, addr, len)       do{}while(0)
    #define HEAP_STAT(detail)               do{}while(0)
    #define STACK_STAT(detail)              do{}while(0)

#elif   (MBED_CONF_APP_RUN_MODE == 1) || (MBED_CONF_APP_RUN_MODE == 2)

    /* Enable serial debug */
    #include "Serial.h"
    extern Serial pc;
    void hex_dump(const char *desc, void *addr, uint32_t len);
    void heap_statistics(bool detail);
    void stack_statistics(bool detail);

    #define DEBUG_SERIAL(...)               Serial pc(__VA_ARGS__)
    #define DEBUG_OUT(...)                  pc.printf(__VA_ARGS__)
    #define DEBUG_IN(...)                   pc.scanf (__VA_ARGS__)
    #define PRESS(c)                        while(pc.getc() != c)
    #define PRESS_ANY_KEY_TO(s)             pc.printf("Press any key to %s >\n", s); while(!(pc.readable() && pc.getc()))
    #define PRESS_ANY_KEY_TO_BREAK_LOOP()   if(pc.readable() && pc.getc()) break
    #define ASSERT(expr) { \
        if(!(expr)) { \
            pc.printf("ERROR: %s ,file %s, line %d \n", #expr, __FILE__, __LINE__); \
            while(1); \
        } \
    }
    #define HEX_DUMP(desc, addr, len)       hex_dump(desc, addr, len)
    #define HEAP_STAT(detail)               heap_statistics(detail)     // NOTE: Require adding the command-line flag -DMBED_HEAP_STATS_ENABLED=1
    #define STACK_STAT(detail)              stack_statistics(detail)    // NOTE: Require adding the command-line flag -DMBED_STACK_STATS_ENABLED=1

#endif

#endif
