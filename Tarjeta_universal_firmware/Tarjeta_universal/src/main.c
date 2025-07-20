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
 * | D22          | PB26           |
 * | D23          | PA14           |
 * | D22          | PA17           |
 * | D23          | PA18           |
 * +--------------+----------------+
 */

#include <asf.h>
#include <twi.h>
#include "encoder.h"
#include "uart_custom.h"
#include "OLED_SSD1306.h"


uint32_t ms_ticks = 0;

void configure_pins(void);
void configure_systick(void);

uint32_t millis(void);

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

int main(void)
{
    sysclk_init();
    board_init();
    configure_systick();

    configure_pins();
    uart_init_default();
    encoder_init();

    configure_pins_TWI();
    ssd1306_init_Default();

    

    // /* 3. Rellenar el buffer con un patrón (alternar columnas) */
    // for (uint32_t i = 0; i < sizeof(g_buffer); ++i)
    //     g_buffer[i] = (i & 1) ? 0xFF : 0x00;

    // /* 4. Mandar el buffer a la pantalla */
    // ssd1306_update();



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
