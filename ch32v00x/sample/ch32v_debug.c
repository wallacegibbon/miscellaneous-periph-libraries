#include <ch32v_debug.h>
#include <ch32v00x.h>

void usart_printf_initialize(uint32_t baudrate) {
	GPIO_InitTypeDef gpio_init;
	USART_InitTypeDef usart_init;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

	gpio_init.GPIO_Pin = GPIO_Pin_5;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &gpio_init);

	usart_init.USART_BaudRate = baudrate;
	usart_init.USART_WordLength = USART_WordLength_8b;
	usart_init.USART_StopBits = USART_StopBits_1;
	usart_init.USART_Parity = USART_Parity_No;
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init.USART_Mode = USART_Mode_Tx;

	USART_Init(USART1, &usart_init);
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

