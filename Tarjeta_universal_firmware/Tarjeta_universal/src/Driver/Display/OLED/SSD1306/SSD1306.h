/*
 * TWI_custom.h
 *
 * Created: 26/07/2025 9:21:59 p. m.
 *  Author: Marquez_diaz
 *
 *  Plantila:
 *
 *  #define DATA   0x00
 *
 *  extern u_int8_t data;  // debe estar definido en el archivo (.c)
 *
 *  void DATA(void);
 *
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include <asf.h>
#include "Driver/Display/OLED/SSD1306/SSD1306_cmd.h"



void configure_pins_TWI_SSD1306(void); 
void Inicialize_SSD1306_default(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght);
void form_hearts(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght);
void New_page(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght,Cmd_START_PAGE page_start_address);
void vaciados(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght);
void frase_A(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght);
void frase_B(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght);
void frase_C(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght);
void scroll(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght );

#endif /* SSD1306_CMD_H_ */