#include <ch32v_debug.h>

void USART_printf_initialize(uint32_t baudrate) {
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

