#ifndef UART_H_
#define UART_H_
#include <asf.h>

void uart_init_default(void);
void uart_puts(Uart *uart, const char *str, size_t len);

#endif