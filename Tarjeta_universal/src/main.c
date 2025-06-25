/*
 * Autor: jose heriberto
 * Fecha de creaccion:
 *
 * Notas:
 * Arduino Due Digital Pin Mapping (D0�D13)
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
 * | D22          | PB26            |
 * | D23          | PA14           |
 
 * +--------------+----------------+
 */

#include <asf.h>
#include "encoder.h"
#include "uart_custom.h"

void configure_pins(void);

/* LEDs dirección ----------------------------------------------------------- */
#define PIN_LED_FWD_IDX PIO_PB26_IDX /* D2  */
#define PIN_LED_FWD_MASK PIO_PB26
#define PIN_LED_FWD_PORT PIOB

#define PIN_LED_REV_IDX PIO_PA14_IDX /* D3  */
#define PIN_LED_REV_MASK PIO_PA14
#define PIN_LED_REV_PORT PIOA

void configure_pins(void)
{
    // Habilitar el reloj para el PIOB
    pmc_enable_periph_clk(ID_PIOB);
    pmc_enable_periph_clk(ID_PIOC);

    // salida
    pio_configure(PIN_LED_FWD_PORT, PIO_OUTPUT_0, PIN_LED_FWD_MASK, PIO_DEFAULT);
    pio_configure(PIN_LED_REV_PORT, PIO_OUTPUT_0, PIN_LED_REV_MASK, PIO_DEFAULT);
}

int main(void)
{
    sysclk_init();
    board_init();
    configure_pins();
    uart_init_default();
    encoder_init();

    while (1)
    {
        /* Cada vuelta actualizamos los LEDs con la última dirección */
        if (encoder_get_direction())
        {
            pio_set(PIN_LED_FWD_PORT, PIN_LED_FWD_MASK);
            pio_clear(PIN_LED_REV_PORT, PIN_LED_REV_MASK);
        }
        else
        {
            pio_clear(PIN_LED_FWD_PORT, PIN_LED_FWD_MASK);
            pio_set(PIN_LED_REV_PORT, PIN_LED_REV_MASK);
        }
    }
}
