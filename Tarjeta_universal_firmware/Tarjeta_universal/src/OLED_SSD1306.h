/*
 * OLED_SSD1306.h
 *
 * Created: 6/07/2025 7:04:54 p. m.
 *  Author: Marquez_diaz
 */ 

 #ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_

#define SSD1306_I2C_ADDR   0x3C
#define SSD1306_WIDTH      128
#define SSD1306_HEIGHT     64
#define SSD1306_PAGES      (SSD1306_HEIGHT / 8)

extern Twi *const OLED_TWI;
extern uint8_t g_buffer[SSD1306_WIDTH * SSD1306_PAGES];

void configure_pins_TWI(void);
void ssd1306_init_Default(void);
void ssd1306_update(void);




#endif /* OLED_SSD1306_H_ */