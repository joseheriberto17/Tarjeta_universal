/*
 * Autor: jose heriberto
 * Fecha de creaccion:
 *
 * Archivo: main.c
 *
 * Notas:
 * Arduino Due Digital Pin Mapping (D0-D13)
 *
 * +--------------+----------------+
 * | Arduino PIN  | SAM3X8E Pin    |
 * +--------------+----------------+
 * | D0           | PA8            |
 * | D1           | PA9            |
 * | D2           | PB25           |
 * | D3           | PC28           |
 * | D4           | PC26/PA29      |
 * | D5           | PC25           |
 * | D6           | PC24           |
 * | D7           | PC23           |
 * | D8           | PC22           |
 * | D9           | PC21           |
 * | D10          | PC29/PA28      |
 * | D11          | PD7            |
 * | D12          | PD8            |
 * | D13 (LED)    | PB27           |
 * | D22          | PB26           |
 * | D23          | PA14           |
 * | D22          | PA17           |
 * | D23          | PA18           |
 * +--------------+----------------+
 */

#include <asf.h>
#include <twi.h>
#include "Driver\Encoder\encoder.h"
#include "Driver\Uart\uart_custom.h"
#include "Driver\Display\OLED\SSD1306\SSD1306.h"
#include "Driver\Display\OLED\SSD1306\SSD1306_cmd.h"
#include <string.h>

#define SSD1306_I2C_ADDR 0x3C
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_PAGES (SSD1306_HEIGHT / 8)

volatile uint32_t ms_ticks = 0;
volatile uint8_t char_data = 0;
volatile uint32_t timer_while = 0;

// BUFFER uart

#define RX_SZ 128
volatile uint8_t rx_q[RX_SZ];
volatile uint16_t rx_head = 0, rx_tail = 0;

void configure_pins(void);
void configure_systick(void);

uint32_t ssd1306_write(const uint8_t *buf, uint32_t len);
uint32_t millis(void);
int rx_get(uint8_t *out);

/* LEDs dirección ----------------------------------------------------------- */
#define PIN_LED_FWD_IDX PIO_PB26_IDX /* D2  */
#define PIN_LED_FWD_MASK PIO_PB26
#define PIN_LED_FWD_PORT PIOB

#define PIN_LED_REV_IDX PIO_PA14_IDX /* D3  */
#define PIN_LED_REV_MASK PIO_PA14
#define PIN_LED_REV_PORT PIOA

// generacion de millis()
void configure_systick(void)
{
    // SystemCoreClock debe estar correctamente definido como 64000000
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        // Error al configurar SysTick
        while (1)
            ;
    }
}

void SysTick_Handler(void)
{
    ms_ticks++;
}

uint32_t millis(void)
{
    return ms_ticks;
}

void configure_pins(void)
{
    // Habilitar el reloj para el PIOB
    pmc_enable_periph_clk(ID_PIOB);
    pmc_enable_periph_clk(ID_PIOC);

    // salida
    pio_configure(PIN_LED_FWD_PORT, PIO_OUTPUT_0, PIN_LED_FWD_MASK, PIO_DEFAULT);
    pio_configure(PIN_LED_REV_PORT, PIO_OUTPUT_0, PIN_LED_REV_MASK, PIO_DEFAULT);
}

uint32_t ssd1306_write(const uint8_t *buf, uint32_t len)
{
    twi_packet_t pkt = {
        .addr_length = 0,
        .chip = SSD1306_I2C_ADDR,
        .buffer = (void *)buf,
        .length = len};
    return twi_master_write(TWI0, &pkt);
}

void UART_Handler(void)
{
    uint32_t sr = uart_get_status(UART);
    if (sr & UART_SR_RXRDY)
    {
        uint8_t b;
        uart_read(UART, &b); // 1 byte
        uint16_t next = (rx_head + 1) % RX_SZ;
        if (next != rx_tail)
        { // si no hay overflow
            rx_q[rx_head] = b;
            rx_head = next;
        } // else: descartar o marcar overflow
    }
}

int rx_get(uint8_t *out)
{
    if (rx_head == rx_tail)
        return 0; // vacío
    *out = rx_q[rx_tail];
    rx_tail = (rx_tail + 1) % RX_SZ;
    return 1;
}

int main(void)
{
    sysclk_init();
    board_init();
    configure_systick();

    configure_pins();
    configure_pins_TWI_SSD1306();
    uart_init_default();

    uint8_t buffer_TWI[64] = {0};
    uint16_t buffer_TWI_lenght = 0;

    Inicialize_SSD1306_default(buffer_TWI, &buffer_TWI_lenght);
    // -------------------debug----------------------------------------
    // uart_puts(UART,(const char *)"\n",1);

    // for (uint8_t j = 0; j < 2 * buffer_TWI_lenght; j++)
    // {
    //     sprintf((char *)buffer_tx, "0x%02x ", buffer_TWI[j]);
    //     uart_puts(UART, (const char *)buffer_tx, strlen((const char *)buffer_tx));
    // }

    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght * 2);

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;

    form_hearts(buffer_TWI, &buffer_TWI_lenght);

    // uart_puts(UART, "\n\n",2);

    // -------------------debug----------------------------------------
    // for (uint16_t j = 0; j < buffer_TWI_lenght; j++)
    // {
    //     sprintf((char *)buffer_tx, "0x%02x ", buffer_TWI[j]);
    //     uart_puts(UART, (const char *)buffer_tx, strlen((const char *)buffer_tx));
    // }

    // sprintf((char *)buffer_tx, "\n%02d",buffer_TWI_lenght);
    // uart_puts(UART, (const char *)buffer_tx, strlen((const char *)buffer_tx));

    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    // memset(buffer_TWI,0,sizeof(buffer_TWI));
    // New_page(buffer_TWI, &buffer_TWI_lenght);
    // ssd1306_write((const uint8_t *)buffer_TWI,(uint32_t) buffer_TWI_lenght);

    //------ corazones inferiores -----------------
    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    New_page(buffer_TWI, &buffer_TWI_lenght, Cmd_START_PAGE_7);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    form_hearts(buffer_TWI, &buffer_TWI_lenght);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    // vaciados 1
    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    New_page(buffer_TWI, &buffer_TWI_lenght, Cmd_START_PAGE_1);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    vaciados(buffer_TWI, &buffer_TWI_lenght);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    // vaciados 2
    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    New_page(buffer_TWI, &buffer_TWI_lenght, Cmd_START_PAGE_2);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    vaciados(buffer_TWI, &buffer_TWI_lenght);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    // vaciado 6
    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    New_page(buffer_TWI, &buffer_TWI_lenght, Cmd_START_PAGE_6);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    vaciados(buffer_TWI, &buffer_TWI_lenght);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    // -------------------------------------------------

    // frase A
    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    New_page(buffer_TWI, &buffer_TWI_lenght, Cmd_START_PAGE_3);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    frase_A(buffer_TWI, &buffer_TWI_lenght);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    // frase B

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    New_page(buffer_TWI, &buffer_TWI_lenght, Cmd_START_PAGE_4);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    frase_B(buffer_TWI, &buffer_TWI_lenght);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    // frase C

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    New_page(buffer_TWI, &buffer_TWI_lenght, Cmd_START_PAGE_5);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    frase_C(buffer_TWI, &buffer_TWI_lenght);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    // scroll
    memset(buffer_TWI, 0, sizeof(buffer_TWI));
    buffer_TWI_lenght = 0;
    scroll(buffer_TWI, &buffer_TWI_lenght);
    ssd1306_write((const uint8_t *)buffer_TWI, (uint32_t)buffer_TWI_lenght);

    timer_while = millis();

    uint8_t buffer_TX[RX_SZ] = {0};
    uint8_t data = " ";

    while (1)
    {

        if ((millis() - timer_while) > 100)
        {
            timer_while = millis();
            rx_get(&data);
            // uart_puts(UART, "llego", 6);

            sprintf((char *)buffer_TX, "data: %c\n", data);
            uart_puts(UART, (const char *)buffer_TX, strlen((const char *)buffer_TX));
        }

        // /* Cada vuelta actualizamos los LEDs con la última dirección */
        // if (encoder_get_direction())
        // {
        //     pio_set(PIN_LED_FWD_PORT, PIN_LED_FWD_MASK);
        //     pio_clear(PIN_LED_REV_PORT, PIN_LED_REV_MASK);
        // }
        // else
        // {
        //     pio_clear(PIN_LED_FWD_PORT, PIN_LED_FWD_MASK);
        //     pio_set(PIN_LED_REV_PORT, PIN_LED_REV_MASK);
        // }
    }
}
