#include "led_clock.h"

uint32_t cvtSimpleTime2Mask(const SimpleTime& time) {
    int16_t minutes = time.minutes % 60;
    const int16_t hours = time.hours % 24;

    uint32_t mask = 0;

    // set hour led
    if (hours > 0) {
        mask = mask | uint32_t{1} << (8 + hours);
    }
   
    // set 45m led
    if (minutes / 45 == 1) {
        mask = mask | (uint32_t{1} << 8);
        minutes -= 45;
    } else if (minutes / 30 == 1) {
        mask = mask | (uint32_t{1} << 7);
        minutes -= 30;
    } else if (minutes / 15 == 1) {
        mask = mask | (uint32_t{1} << 6);
        minutes -= 15;
    }

    // set 10m led
    if (minutes / 10 == 1) {
        mask = mask | (uint32_t{1} << 5);
        minutes -= 10;
    } else if (minutes / 5 == 1) {
        mask = mask | (uint32_t{1} << 4);
        minutes -= 5;
    }

    // set 1m led
    if (minutes == 4) {
        mask = mask | (uint32_t{1} << 3);
    } else if (minutes == 3) {
        mask = mask | (uint32_t{1} << 2);
    } else if (minutes == 2) {
        mask = mask | (uint32_t{1} << 1);
    } else if (minutes == 1) {
        mask = mask | (uint32_t{1} << 0);
    }

    return mask;
}
