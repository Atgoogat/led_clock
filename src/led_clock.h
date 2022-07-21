#ifndef LED_CLOCK_H
#define LED_CLOCK_H

#include <stdint.h>

struct SimpleTime {
    uint8_t minutes; 
    uint8_t hours; 
};

uint32_t cvtSimpleTime2Mask(const SimpleTime&);

#endif