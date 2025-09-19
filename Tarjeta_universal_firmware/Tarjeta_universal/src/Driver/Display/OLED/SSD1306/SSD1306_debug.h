/*
 * SSD1306_debug.h
 *
 * Created: 8/09/2025 8:24:04 p. m.
 *  Author: Marquez_diaz
 */ 


#ifndef SSD1306_DEBUG_H_
#define SSD1306_DEBUG_H_

#include <asf.h>
#include "Driver/Uart/uart_custom.h"

#include "Driver/Display/OLED/SSD1306/SSD1306_cmd.h"

void select_cmd(u_int8_t *buf, u_int8_t *buf_size, uint8_t select_cmd, uint8_t *p_Args, uint8_t len_Args);




#endif /* SSD1306_DEBUG_H_ */