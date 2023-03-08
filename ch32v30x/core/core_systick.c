#include "core_systick.h"
#include <ch32v30x.h>

static volatile uint32_t millis_count = 0;

void initialize_systick_interrupt() {
	NVIC_InitTypeDef NVIC_InitStructure;
	uint32_t increment_per_milli;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	increment_per_milli = SystemCoreClock / 8000;

	SysTick->CTLR = 0;
	SysTick->SR = 0;
	SysTick->CNT = 0;
	SysTick->CMP = increment_per_milli;
	SysTick->CTLR = 0xb;
}

uint32_t micros() {
	return millis_count * 1000 + SysTick->CNT * 1000 / SysTick->CMP;
}

uint32_t millis() {
	return millis_count;
}

void delay_us(uint32_t micro_seconds) {
	uint32_t prev_micros;
	prev_micros = micros();
	while (micros() - prev_micros < micro_seconds);
}

void delay_ms(uint32_t milli_seconds) {
	uint32_t prev_millis;
	prev_millis = millis();
	while (millis() - prev_millis < milli_seconds);
}

__attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler() {
	SysTick->SR = 0;
	millis_count++;
}

