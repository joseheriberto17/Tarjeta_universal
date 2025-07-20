/*
 * OLED_SSD1306.c
 *
 * Created: 6/07/2025 7:05:56 p. m.
 *  Author: Marquez_diaz
 */ 
#include <asf.h>
#include "OLED_SSD1306.h"

Twi *const OLED_TWI = TWI0;
uint8_t g_buffer[SSD1306_WIDTH * SSD1306_PAGES];   // VRAM local
uint32_t ssd1306_write(const uint8_t *buf, uint32_t len);
void ssd1306_cmd(uint8_t cmd);
void ssd1306_data(const uint8_t *data, uint32_t len);

// funciones privados

uint32_t ssd1306_write(const uint8_t *buf, uint32_t len)
{
	twi_packet_t pkt = {
		.addr_length = 0,
		.chip        = SSD1306_I2C_ADDR,
		.buffer      = (void *)buf,
		.length      = len
	};
	return twi_master_write(OLED_TWI, &pkt);
}

void ssd1306_cmd(uint8_t cmd)
{
    uint8_t frame[2] = {0x00, cmd};     // 0x00 = “control byte” → comando
    ssd1306_write(frame, 2);
}

void ssd1306_data(const uint8_t *data, uint32_t len)
{
    uint8_t frame[1] = {0x40};          // 0x40 = “control byte” → datos
    twi_packet_t pkt = {
        .addr_length = 0,
        .chip        = SSD1306_I2C_ADDR,
        .buffer      = (void *)data,
        .length      = len
    };
    /* Enviamos primero el byte de control 0x40 */
    ssd1306_write(frame, 1);
    twi_master_write(OLED_TWI, &pkt);
}



// funciones publicas

void ssd1306_update(void)
{
    for (uint8_t page = 0; page < SSD1306_PAGES; ++page) {
        ssd1306_cmd(0xB0 | page);        // Set Page address
        ssd1306_cmd(0x00);               // Lower column start
        ssd1306_cmd(0x10);               // Higher column start
        ssd1306_data(&g_buffer[page * SSD1306_WIDTH], SSD1306_WIDTH);
    }
}

void ssd1306_init_Default(void)
{
    /* Secuencia idéntica a tus “i2cset …” */
    const uint8_t init_cmds[] = {
        0xAE,             // Display OFF
        0xA8, 0x3F,       // Multiplex ratio a 1/64
        0xD3, 0x00,       // Offset 0
        0x40,             // Start Line 0
        0xA1,             // Segment remap (columna 127→0)
        0xC8,             // COM scan dir remapped
        0xDA, 0x02,       // HW config: sequential, disable COM left/right remap
        0x81, 0x7F,       // Contraste
        0xA4,             // Seguir contenido de RAM (no “all on”)
        0xA6,             // Normal (no invertido)
        0xD5, 0x80,       // Freq/osc divisor
        0x8D, 0x14,       // Habilita charge-pump interno
        0xAF              // Display ON
    };

    for (uint32_t i = 0; i < sizeof(init_cmds); ++i)
        ssd1306_cmd(init_cmds[i]);
}

 void configure_pins_TWI(void)
{
    // Habilitar el reloj para el PIOB
    pmc_enable_periph_clk(ID_TWI0);
    pmc_enable_periph_clk(ID_PIOA);

    // salida
    pio_configure(PIOA, PIO_PERIPH_A, PIO_PA18 | PIO_PA17, PIO_DEFAULT);

    twi_options_t opts = {
        .speed = 100000,
        .smbus = 0, // i2c clasico
        .master_clk = sysclk_get_cpu_hz() 
    };

    twi_master_init(TWI0,&opts);
}
