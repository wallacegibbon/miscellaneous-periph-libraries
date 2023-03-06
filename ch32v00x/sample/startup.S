	.section .init, "ax", @progbits
	.global _start
	.align 2

	.macro IRQ handler
	.word \handler
	.weak \handler
	.set \handler, default_interrupt_handler
	.endm

_start:
	.option norvc
	j handle_reset
	.word 0
	IRQ NMI_Handler
	IRQ HardFault_Handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	IRQ SysTick_Handler
	.word 0
	IRQ SW_Handler
	.word 0
	/* External Interrupts */
	IRQ WWDG_IRQHandler
	IRQ PVD_IRQHandler
	IRQ FLASH_IRQHandler
	IRQ RCC_IRQHandler
	IRQ EXTI7_0_IRQHandler
	IRQ AWU_IRQHandler
	IRQ DMA1_Channel1_IRQHandler
	IRQ DMA1_Channel2_IRQHandler
	IRQ DMA1_Channel3_IRQHandler
	IRQ DMA1_Channel4_IRQHandler
	IRQ DMA1_Channel5_IRQHandler
	IRQ DMA1_Channel6_IRQHandler
	IRQ DMA1_Channel7_IRQHandler
	IRQ ADC1_IRQHandler
	IRQ I2C1_EV_IRQHandler
	IRQ I2C1_ER_IRQHandler
	IRQ USART1_IRQHandler
	IRQ SPI1_IRQHandler
	IRQ TIM1_BRK_IRQHandler
	IRQ TIM1_UP_IRQHandler
	IRQ TIM1_TRG_COM_IRQHandler
	IRQ TIM1_CC_IRQHandler
	IRQ TIM2_IRQHandler

	.section .text.default_interrupt_handler, "ax", @progbits
default_interrupt_handler:
	j default_interrupt_handler
	/*
	mret
	*/

	.option rvc

	.section .text.handle_reset, "ax", @progbits
	.weak handle_reset
	.align 1
handle_reset:
	.option push
	.option norelax
	la gp, __global_pointer$
	.option pop
1:
	la sp, _eusrstack
2:
	/* Load data section from flash to RAM */
	la a0, _data_lma
	la a1, _data_vma
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:
	/* clear bss section */
	la a0, _sbss
	la a1, _ebss
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
	li t0, 0x80
	csrw mstatus, t0

	li t0, 0x3
	csrw 0x804, t0

	la t0, _start
	ori t0, t0, 3
	csrw mtvec, t0

	jal SystemInit
	la t0, main
	csrw mepc, t0
	mret

