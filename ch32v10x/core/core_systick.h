#ifndef __CORE_SYSTICK_H
#define __CORE_SYSTICK_H

#include <stdint.h>

int systick_interrupt_init(void);

uint32_t micros(void);
uint32_t millis(void);

int delay_us(uint32_t micro_seconds);
int delay_ms(uint32_t milli_seconds);

#endif
