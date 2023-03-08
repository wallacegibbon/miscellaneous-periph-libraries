#include <ch32v_debug.h>

static uint16_t p_ms = 0;
static uint8_t p_us = 0;

void Delay_Init(void) {
	p_us = SystemCoreClock / 8000000;
	p_ms = (uint16_t) p_us * 1000;
}

void Delay_Us(uint32_t n) {
	uint32_t i;

	SysTick->SR &= ~(1 << 0);
	i = (uint32_t) n * p_us;

	SysTick->CMP = i;
	SysTick->CNT = 0;
	SysTick->CTLR |= (1 << 0);

	while ((SysTick->SR & (1 << 0)) != (1 << 0));
	SysTick->CTLR &= ~(1 << 0);
}

void Delay_Ms(uint32_t n) {
	uint32_t i;

	SysTick->SR &= ~(1 << 0);
	i = (uint32_t) n * p_ms;

	SysTick->CMP = i;
	SysTick->CNT = 0;
	SysTick->CTLR |= (1 << 0);

	while ((SysTick->SR & (1 << 0)) != (1 << 0));
	SysTick->CTLR &= ~(1 << 0);
}

void USART_Printf_Init(uint32_t baudrate) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

__attribute__((used))
int _write(int fd, char *buf, int size) {
	int i;

	for (i = 0; i < size; i++) {
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, *buf++);
	}

	return size;
}

void *_sbrk(ptrdiff_t incr) {
	extern char _end[];
	extern char _heap_end[];
	static char *curbrk = _end;

	if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
		return NULL - 1;

	curbrk += incr;
	return curbrk - incr;
}

