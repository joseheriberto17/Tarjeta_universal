#include "uart_custom.h"

void uart_init_default(void)
{
    pmc_enable_periph_clk(ID_UART);
    pio_configure(PINS_UART_PIO, PINS_UART_TYPE, PINS_UART_MASK, PINS_UART_ATTR);

    sam_uart_opt_t opts = {
        .ul_mck      = sysclk_get_cpu_hz(),
        .ul_baudrate = 9600,
        .ul_mode     = UART_MR_PAR_NO
    };

    uart_init(UART, &opts);
    uart_enable_tx(UART);
    uart_enable_rx(UART);
}

void uart_puts(Uart *uart, const char *str, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        while (!uart_is_tx_ready(uart));
        uart_write(uart, (uint8_t) str[i]);
    }
}
