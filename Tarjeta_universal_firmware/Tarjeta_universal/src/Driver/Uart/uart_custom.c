#include "uart_custom.h"

void uart_init_default(void)
{
    pmc_enable_periph_clk(ID_UART);
    pmc_enable_periph_clk(ID_PIOA);

    // RX (URXD, PA8) con pull-up
    pio_configure(PIOA, PIO_PERIPH_A, PIO_PA8A_URXD, PIO_PULLUP);

    // TX (UTXD, PA9) sin pull-up
    pio_configure(PIOA, PIO_PERIPH_A, PIO_PA9A_UTXD, PIO_DEFAULT);

    sam_uart_opt_t opts = {
        .ul_mck = sysclk_get_cpu_hz(),
        .ul_baudrate = 9600,
        .ul_mode = UART_MR_PAR_NO};

    uart_init(UART, &opts);

    /* Habilitar RX y TX */
    uart_enable_rx(UART);
    uart_enable_tx(UART);

    /* Habilitar interrupción RXRDY y NVIC */
    uart_enable_interrupt(UART, UART_IER_RXRDY);
    NVIC_ClearPendingIRQ(UART_IRQn);
    NVIC_SetPriority(UART_IRQn, 3);
    NVIC_EnableIRQ(UART_IRQn);
}

void uart_puts(Uart *uart, const char *str, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        while (!uart_is_tx_ready(uart))
            ;
        uart_write(uart, (uint8_t)str[i]);
    }
}
