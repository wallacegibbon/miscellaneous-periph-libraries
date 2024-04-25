#include "core_systick.h"
#include <ch32v20x.h>

void SysTick_Handler() __attribute__((interrupt("WCH-Interrupt-fast")));

static volatile uint32_t millis_count = 0;

int systick_interrupt_init() {
	NVIC_InitTypeDef nvic_init;
	uint32_t increment_per_milli;

	nvic_init.NVIC_IRQChannel = SysTick_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init.NVIC_IRQChannelSubPriority = 0;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic_init);

	increment_per_milli = SystemCoreClock / 8000;

	SysTick->CTLR = 0;
	SysTick->SR = 0;
	SysTick->CNT = 0;
	SysTick->CMP = increment_per_milli;
	SysTick->CTLR = 0xb;
	return 0;
}

uint32_t micros() {
	return millis_count * 1000 + SysTick->CNT * 1000 / SysTick->CMP;
}

uint32_t millis() {
	return millis_count;
}

int delay_us(uint32_t micro_seconds) {
	uint32_t prev_micros;
	prev_micros = micros();
	while (micros() - prev_micros < micro_seconds)
		;
	return 0;
}

int delay_ms(uint32_t milli_seconds) {
	uint32_t prev_millis;
	prev_millis = millis();
	while (millis() - prev_millis < milli_seconds)
		;
	return 0;
}

void SysTick_Handler() {
	SysTick->SR = 0;
	millis_count++;
}
