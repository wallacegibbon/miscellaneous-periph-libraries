#include <stddef.h>
#include <stdint.h>

extern unsigned char _eusrstack;
extern unsigned char _data_vma;
extern unsigned char _edata;
extern unsigned char _data_lma;
extern unsigned char _sbss;
extern unsigned char _ebss;

/*
typedef void (*function_t)();

extern function_t __init_array_start[0];
extern function_t __init_array_end[0];
*/

int main(void);

void Reset_Handler(void)
{
	// function_t *tmp_fn;
	unsigned char *tmp;
	size_t data_size;
	size_t i;

	data_size = &_edata - &_data_vma;
	for (i = 0; i < data_size; i++)
		*((unsigned char *)&_data_vma + i) = *((unsigned char *)&_data_lma + i);

	/// Initialize bss section
	for (tmp = &_sbss; tmp < &_ebss; tmp++)
		*tmp = 0;

	/*
	/// Initialize static objects by calling their constructors
	for (tmp_fn = __init_array_start; tmp_fn < __init_array_end; tmp_fn++)
		(*tmp_fn)();
	*/

	main();
}

void DefaultISR(void)
{
	while (1)
		;
}

#define DEFINE_DEFAULT_ISR(name) \
	__attribute__((weak)) void name(void) { DefaultISR(); }

DEFINE_DEFAULT_ISR(NMI_Handler)
DEFINE_DEFAULT_ISR(HardFault_Handler)
DEFINE_DEFAULT_ISR(SVC_Handler)
DEFINE_DEFAULT_ISR(DebugMon_Handler)
DEFINE_DEFAULT_ISR(PendSV_Handler)
DEFINE_DEFAULT_ISR(SysTick_Handler)
DEFINE_DEFAULT_ISR(FLASH_IRQHandler)
DEFINE_DEFAULT_ISR(RCC_IRQHandler)
DEFINE_DEFAULT_ISR(EXTI0_1_IRQHandler)
DEFINE_DEFAULT_ISR(EXTI2_3_IRQHandler)
DEFINE_DEFAULT_ISR(EXTI4_15_IRQHandler)
DEFINE_DEFAULT_ISR(ADC_COMP_IRQHandler)
DEFINE_DEFAULT_ISR(TIM1_BRK_UP_TRG_COM_IRQHandler)
DEFINE_DEFAULT_ISR(TIM1_CC_IRQHandler)
DEFINE_DEFAULT_ISR(LPTIM1_IRQHandler)
DEFINE_DEFAULT_ISR(TIM16_IRQHandler)
DEFINE_DEFAULT_ISR(I2C1_IRQHandler)
DEFINE_DEFAULT_ISR(SPI1_IRQHandler)
DEFINE_DEFAULT_ISR(USART1_IRQHandler)

uintptr_t exception_table[48] __attribute__((section(".isr_vector"))) = {
	(uintptr_t)&_eusrstack,
	(uintptr_t)Reset_Handler,
	(uintptr_t)NMI_Handler,
	(uintptr_t)HardFault_Handler,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	(uintptr_t)SVC_Handler,
	NULL,
	NULL,
	(uintptr_t)PendSV_Handler,
	(uintptr_t)SysTick_Handler,
	NULL,
	NULL,
	NULL,
	(uintptr_t)FLASH_IRQHandler,
	(uintptr_t)RCC_IRQHandler,
	(uintptr_t)EXTI0_1_IRQHandler,
	(uintptr_t)EXTI2_3_IRQHandler,
	(uintptr_t)EXTI4_15_IRQHandler,
	NULL,
	NULL,
	NULL,
	NULL,
	(uintptr_t)ADC_COMP_IRQHandler,
	(uintptr_t)TIM1_BRK_UP_TRG_COM_IRQHandler,
	(uintptr_t)TIM1_CC_IRQHandler,
	NULL,
	NULL,
	(uintptr_t)LPTIM1_IRQHandler,
	NULL,
	NULL,
	NULL,
	(uintptr_t)TIM16_IRQHandler,
	NULL,
	(uintptr_t)I2C1_IRQHandler,
	NULL,
	(uintptr_t)SPI1_IRQHandler,
	NULL,
	(uintptr_t)USART1_IRQHandler,
	NULL,
	NULL,
	NULL,
	NULL,
};
