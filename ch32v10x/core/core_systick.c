#include "core_systick.h"
#include <ch32v10x.h>

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

static volatile uint32_t millis_count = 0;

int systick_interrupt_init(void)
{
	NVIC_InitTypeDef nvic_init;
	uint32_t increment_per_milli;

	nvic_init.NVIC_IRQChannel = SysTick_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init.NVIC_IRQChannelSubPriority = 0;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic_init);

	increment_per_milli = SystemCoreClock / 8000;

	SysTick->CTLR = 0;

	SysTick->CNTL0 = 0;
	SysTick->CNTL1 = 0;
	SysTick->CNTL2 = 0;
	SysTick->CNTL3 = 0;

	SysTick->CMPLR0 = increment_per_milli;
	SysTick->CMPLR1 = increment_per_milli >> 8;
	SysTick->CMPLR2 = increment_per_milli >> 16;
	SysTick->CMPLR3 = increment_per_milli >> 24;

	/// it's necessary to write 0 to CMPHR (not sure why)
	SysTick->CMPHR0 = 0;
	SysTick->CMPHR1 = 0;
	SysTick->CMPHR2 = 0;
	SysTick->CMPHR3 = 0;

	SysTick->CTLR = 1;
	return 0;
}

uint32_t micros(void)
{
	uint32_t cnt, cmp;
	cnt = *((uint32_t *)&SysTick->CNTL0);
	cmp = *((uint32_t *)&SysTick->CMPLR0);
	return millis_count * 1000 + cnt * 1000 / cmp;
}

uint32_t millis(void)
{
	return millis_count;
}

int delay_us(uint32_t micro_seconds)
{
	uint32_t prev_micros;
	prev_micros = micros();
	while (micros() - prev_micros < micro_seconds)
		;
	return 0;
}

int delay_ms(uint32_t milli_seconds)
{
	uint32_t prev_millis;
	prev_millis = millis();
	while (millis() - prev_millis < milli_seconds)
		;
	return 0;
}

void SysTick_Handler(void)
{
	SysTick->CNTL0 = 0;
	SysTick->CNTL1 = 0;
	SysTick->CNTL2 = 0;
	SysTick->CNTL3 = 0;
	millis_count++;
}
