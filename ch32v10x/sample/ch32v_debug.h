#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>

#define DEBUG_UART1 1
#define DEBUG_UART2 2
#define DEBUG_UART3 3

#ifndef DEBUG
#define DEBUG DEBUG_UART1
#endif

int usart_printf_initialize(unsigned long baudrate);

#endif
