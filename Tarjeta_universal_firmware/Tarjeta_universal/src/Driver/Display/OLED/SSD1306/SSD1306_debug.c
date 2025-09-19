/*
 * SSD1306_debug.c
 *
 * Created: 8/09/2025 8:23:26 p. m.
 *  Author: Marquez_diaz
 */ 

#include "Driver/Display/OLED/SSD1306/SSD1306_debug.h"




void print_list_cmd(u_int8_t *buf, u_int8_t *buf_size)
{
    uart_puts(UART, (const char *)buf, strlen((const char *)buf_size));
}

// len_Args , debe se igua la los argumentos
void select_cmd(u_int8_t *buf, u_int8_t *buf_size, uint8_t select_cmd, uint8_t *p_Args, uint8_t len_Args)
{
    switch (select_cmd)
    {
    case 1:
        if (validar_longitud_Args(len_Args,1)) 
        return;    

        Set_Contrast_Control(*buf,*buf_size,*p_Args);
        break;
    
    default:
        break;
    }
}



uint8_t validar_longitud_Args(uint8_t len_Args_cmd_select, uint8_t len_Args )
{
    if (len_Args_cmd_select == len_Args)
    {
       return 0;
    }

    return 1;
    
}