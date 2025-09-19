/*
 * TWI_custom.c
 *
 * Created: 26/07/2025 9:23:35 p. m.
 *  Author: Marquez_diaz
 *
 *
 *  platilla:
 *
 *  #include "data.h"
 *
 *
 *  twi_packet_t pkt = {
 *		.addr_length = 0,
 *		.chip        = (direccion de receptor),
 *		.buffer_TWI      = buffer_TWI,
 *		.length      = len
 *	};
 *
 *  twi_master_write(periferico_base, &pkt):
 *
 *  ya tiene una funcion de inicializacion
 *
 */

#include "Driver\Display\OLED\SSD1306\SSD1306.h"

uint8_t form_heart[8] = {0x1c,0x22,0x41,0x82,0x82,0x41,0x22,0x1c};
uint8_t fraseA[128] = {0};
uint8_t fraseB[128] = {0};
uint8_t fraseC[128] = {0};

// Letra Z
// Letra A
uint8_t letras_A[8] = { 
    0xF8, // #####   
    0xFE, // ####### 
    0x36, //   ## ## 
    0x33, //   ##  ##
    0x36, //   ## ## 
    0xFE, // ####### 
    0xF8, // #####   
    0x00
};

// Letra Z
uint8_t letras_Z[8] = {
    0xE3, // ###   ##
    0xF3, // ####  ##
    0xFD, // ##### ##
    0xFF, // ########
    0xDf, // ## #####
    0xC7, // ##   ###
    0xC3, // ##    ##
    0x00


};

// Letra H
uint8_t letras_H[8] = {
    0xFF, // ########
    0xFF, // ########
    0x18, //    ##   
    0x18, //    ##   
    0x18, //    ##   
    0xFF, // ########
    0xFF, // ########
    0x00  
};

// Letra R
uint8_t letras_R[8] = {
    0xFF, // ########
    0xFF, // ########
    0x31, //   ##   #
    0x3b, //   ### ##
    0x7F, //  #######
    0xEE, // ### ###
    0xc4, // ##   #  
    0x00

};

// Letra I
uint8_t letras_I[8] = {
    0x00, //         
    0x63, //  ##   ##
    0xFF, // ########
    0xFF, // ########
    0x63, //  ##   ##
    0x00, //         
    0x00, //         
    0x00  //         
};

// Letra T
uint8_t letras_T[8] = {
    0x03, //       ##
    0x03, //       ##
    0xFF, // ########
    0xFF, // ########
    0x03, //       ##
    0x03, //       ##
    0x03, //       ##
    0x00  //       
};

// Letra M
uint8_t letras_M[8] = {
    0xFF, // ########
    0xFF, // ########
    0x0E, //     ### 
    0x1C, //    ###  
    0x0E, //     ### 
    0xFF, // ########
    0xFF, // ########
    0x00 
};

// Letra P
uint8_t letras_P[8] = {
    0xFF, // ########
    0xFF, // ########
    0x33, //   ##  ##
    0x33, //   ##  ##
    0x3F, //   ######
    0x1E, //    #### 
    0x0C, //     ##  
    0x00  //         
};

// Letra E
uint8_t letras_E[8] = {
    0xFF, // ########
    0xFF, // ########
    0xDB, // ## ## ##
    0xDB, // ## ## ##
    0xDB, // ## ## ##
    0xC3, // ##    ##
    0xC3, // ##    ##
    0x00

};

// Letra N
uint8_t letras_N[8] = {
    0xFF, // ########
    0xFF, // ########
    0x06, //      ## 
    0x0C, //     ##  
    0x18, //    ##   
    0xFF, // ########
    0xFF, // ########
    0x00
};

// Letra D
uint8_t letras_D[8] = {
    0xFF, // ########
    0xFF, // ########
    0xC3, // ##    ##
    0xC3, // ##    ##
    0x66, //  ##  ## 
    0x3C, //   ####  
    0x18, //    ##   
    0x00  //         
};

// Letra J
uint8_t letras_J[8] = {
    0x63, //  ##   ##
    0xe3, // ###   ##
    0xc3, // ##    ##
    0xff, // ########
    0x7f, //  #######
    0x03, //       ##
    0x03, //       ##
    0x00  //
};




void configure_pins_TWI_SSD1306(void)
{
    // Habilitar el reloj para el PIOB
    pmc_enable_periph_clk(ID_TWI0);
    pmc_enable_periph_clk(ID_PIOA);

    // salida
    pio_configure(PIOA, PIO_PERIPH_A, PIO_PA18 | PIO_PA17, PIO_DEFAULT);

    twi_options_t opts = {
        .speed = 100000,
        .smbus = 0, // i2c clasico
        .master_clk = sysclk_get_cpu_hz()};

    twi_master_init(TWI0, &opts);
}



void Inicialize_SSD1306_default(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght)
{
    uint8_t Counter = 0;
    uint8_t cmd_lenght[16] = {0};
    uint8_t buffer_cmd[16][7] = {0};
    uint8_t buffer_cmd_lenght = 7;    

    cmd_lenght[Counter] = Set_Display_ON_OFF(buffer_cmd[Counter], buffer_cmd_lenght, false);
    Counter++;

    cmd_lenght[Counter] = Set_Multiplex_Ratio(buffer_cmd[Counter], buffer_cmd_lenght, 0x3F);
    Counter++;

    cmd_lenght[Counter] = Set_Display_Offset(buffer_cmd[Counter], buffer_cmd_lenght, 0x00);
    Counter++;

    cmd_lenght[Counter] = Set_Display_Start_Line(buffer_cmd[Counter], buffer_cmd_lenght, 0x00);
    Counter++;

    cmd_lenght[Counter] = Set_Segment_Remap(buffer_cmd[Counter], buffer_cmd_lenght, true);
    Counter++;

    cmd_lenght[Counter] = Set_COM_Output_Scan_Direction(buffer_cmd[Counter], buffer_cmd_lenght, true);
    Counter++;

    cmd_lenght[Counter] = Set_COM_Pins_Hardware_Configuration(buffer_cmd[Counter], buffer_cmd_lenght, true, false);
    Counter++;

    cmd_lenght[Counter] = Set_Contrast_Control(buffer_cmd[Counter], buffer_cmd_lenght, 0x7F);
    Counter++;

    cmd_lenght[Counter] = Entire_Display_ON(buffer_cmd[Counter], buffer_cmd_lenght, false);
    Counter++;

    cmd_lenght[Counter] = Set_N_I_Display(buffer_cmd[Counter], buffer_cmd_lenght, false);
    Counter++;

    cmd_lenght[Counter] = Set_Display_Clock_Divide_Ratio_and_OSC_Frequency(buffer_cmd[Counter], buffer_cmd_lenght, 0x00, 0x08);
    Counter++;

    cmd_lenght[Counter] = Charge_Pump_Command(buffer_cmd[Counter], buffer_cmd_lenght, true);
    Counter++;

    cmd_lenght[Counter] = Set_Display_ON_OFF(buffer_cmd[Counter], buffer_cmd_lenght, true);
    Counter++;

    //----------------
    // cmd_lenght[Counter] = Set_Memory_Addressing_Mode(buffer_cmd[Counter], buffer_cmd_lenght, Cmd_MEMORY_MODE_HORIZONTAL);
    // Counter++;

    for (uint8_t j = 0; j < Counter; j++)
    {
        for (uint8_t i = 0; i < cmd_lenght[j]; i++)
        {
            if (i == cmd_lenght[j]-1 && j == (Counter - 1))
            {
                buffer_TWI[2 * (*buffer_TWI_lenght + i)] = 0x00;
            }
            else
            {
                buffer_TWI[2 * (*buffer_TWI_lenght + i)] = 0x80;
            }
            buffer_TWI[2 * (*buffer_TWI_lenght + i) + 1] = buffer_cmd[j][i];
        }
        *buffer_TWI_lenght += cmd_lenght[j];
    }
}

void form_hearts(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght)
{
    *buffer_TWI_lenght = 256;
    uint8_t counter = 0;
    
    // for (uint8_t j = 0; j < 8; j++)
    // {
        for (uint16_t i = 0; i < *buffer_TWI_lenght/2; i++)
        {
            if (i == *buffer_TWI_lenght/2-1)
            {
                buffer_TWI[2 *i] = 0x40;
            }
            else
            {
                buffer_TWI[2 *i] = 0xC0;
            }

            buffer_TWI[2*i + 1] = form_heart[counter];

            if (counter == 8)
            {
                counter = 0;
            } 
            else
            {
                counter++;
            }    
        }
    // }
}


void New_page(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght,Cmd_START_PAGE page_start_address )
{
    uint8_t Counter = 0;
    uint8_t cmd_lenght[16] = {0};
    uint8_t buffer_cmd[16][7] = {0};
    uint8_t buffer_cmd_lenght = 7;    

    // cmd_lenght[Counter] = Activate_Scroll(buffer_cmd[Counter], buffer_cmd_lenght);
    // Counter++;
    cmd_lenght[Counter] = Set_Page_Start_Address_for_Page_Addressing_Mode(buffer_cmd[Counter], buffer_cmd_lenght, page_start_address);
    Counter++;

    for (uint8_t j = 0; j < Counter; j++)
    {
        for (uint8_t i = 0; i < cmd_lenght[j]; i++)
        {
            if (i == cmd_lenght[j]-1 && j == (Counter - 1))
            {
                buffer_TWI[2 * (*buffer_TWI_lenght + i)] = 0x00;
            }
            else
            {
                buffer_TWI[2 * (*buffer_TWI_lenght + i)] = 0x80;
            }
            buffer_TWI[2 * (*buffer_TWI_lenght + i) + 1] = buffer_cmd[j][i];
        }
        *buffer_TWI_lenght += cmd_lenght[j];
    }
}

void vaciados(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght)
{
    *buffer_TWI_lenght = 256;
    uint8_t counter = 0;
    
    // for (uint8_t j = 0; j < 8; j++)
    // {
        for (uint16_t i = 0; i < *buffer_TWI_lenght/2; i++)
        {
            if (i == *buffer_TWI_lenght/2-1)
            {
                buffer_TWI[2 *i] = 0x40;
            }
            else
            {
                buffer_TWI[2 *i] = 0xC0;
            }

            buffer_TWI[2*i + 1] = 0x00;

            if (counter == 8)
            {
                counter = 0;
            } 
            else
            {
                counter++;
            }    
        }
    // }
}

void frase_A(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght)
{
    *buffer_TWI_lenght = 256;

    memcpy(fraseA+8*(0+4),letras_Z,sizeof(letras_Z));
    memcpy(fraseA+8*(1+4),letras_H,sizeof(letras_H));
    memcpy(fraseA+8*(2+4),letras_A,sizeof(letras_A));
    memcpy(fraseA+8*(3+4),letras_R,sizeof(letras_R));
    memcpy(fraseA+8*(4+4),letras_I,sizeof(letras_I));
    memcpy(fraseA+8*(5+4),letras_T,sizeof(letras_T));
    memcpy(fraseA+8*(6+4),letras_H,sizeof(letras_H));
    
    
    // for (uint8_t j = 0; j < 8; j++)
    // {
        for (uint16_t i = 0; i < *buffer_TWI_lenght/2; i++)
        {
            if (i == *buffer_TWI_lenght/2-1)
            {
                buffer_TWI[2 *i] = 0x40;
            }
            else
            {
                buffer_TWI[2 *i] = 0xC0;
            }

            buffer_TWI[2*i + 1] = fraseA[i];  
        }
    // }
}

void frase_B(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght)
{
    *buffer_TWI_lenght = 256;

    memcpy(fraseB+8*(0+7),letras_M,sizeof(letras_M));
    memcpy(fraseB+8*(1+7),letras_I,sizeof(letras_I));
    
    
    // for (uint8_t j = 0; j < 8; j++)
    // {
        for (uint16_t i = 0; i < *buffer_TWI_lenght/2; i++)
        {
            if (i == *buffer_TWI_lenght/2-1)
            {
                buffer_TWI[2 *i] = 0x40;
            }
            else
            {
                buffer_TWI[2 *i] = 0xC0;
            }

            buffer_TWI[2*i + 1] = fraseB[i];  
        }
    // }
}

void frase_C(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght)
{
    *buffer_TWI_lenght = 256;

    memcpy(fraseC+8*(0+5),letras_P,sizeof(letras_P));
    memcpy(fraseC+8*(1+5),letras_E,sizeof(letras_E));
    memcpy(fraseC+8*(2+5),letras_N,sizeof(letras_N));
    memcpy(fraseC+8*(3+5),letras_D,sizeof(letras_D));
    memcpy(fraseC+8*(4+5),letras_E,sizeof(letras_E));
    memcpy(fraseC+8*(5+5),letras_J,sizeof(letras_J));
    memcpy(fraseC+8*(6+5),letras_A,sizeof(letras_A));
    
    
    // for (uint8_t j = 0; j < 8; j++)
    // {
        for (uint16_t i = 0; i < *buffer_TWI_lenght/2; i++)
        {
            if (i == *buffer_TWI_lenght/2-1)
            {
                buffer_TWI[2 *i] = 0x40;
            }
            else
            {
                buffer_TWI[2 *i] = 0xC0;
            }

            buffer_TWI[2*i + 1] = fraseC[i];  
        }
    // }
}


void scroll(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght )
{
    uint8_t Counter = 0;
    uint8_t cmd_lenght[16] = {0};
    uint8_t buffer_cmd[16][7] = {0};
    uint8_t buffer_cmd_lenght = 7;    

    cmd_lenght[Counter] = Continuous_Horizontal_Scroll(buffer_cmd[Counter], buffer_cmd_lenght,1, Cmd_START_PAGE_0,Cmd_SET_FRAME_256, Cmd_START_PAGE_7);
    Counter++;                                   
    cmd_lenght[Counter] = Activate_Scroll(buffer_cmd[Counter], buffer_cmd_lenght);
    Counter++;
    

    for (uint8_t j = 0; j < Counter; j++)
    {
        for (uint8_t i = 0; i < cmd_lenght[j]; i++)
        {
            if (i == cmd_lenght[j]-1 && j == (Counter - 1))
            {
                buffer_TWI[2 * (*buffer_TWI_lenght + i)] = 0x00;
            }
            else
            {
                buffer_TWI[2 * (*buffer_TWI_lenght + i)] = 0x80;
            }
            buffer_TWI[2 * (*buffer_TWI_lenght + i) + 1] = buffer_cmd[j][i];
        }
        *buffer_TWI_lenght += cmd_lenght[j];
    }
}



// void New_page(uint8_t *buffer_TWI, uint16_t *buffer_TWI_lenght)
// {
//     uint8_t Counter = 0;
//     uint8_t cmd_lenght[16] = {0};
//     uint8_t buffer_cmd[16][7] = {0};
//     uint8_t buffer_cmd_lenght = 7;    

//     // cmd_lenght[Counter] = Activate_Scroll(buffer_cmd[Counter], buffer_cmd_lenght);
//     // Counter++;
//     cmd_lenght[Counter] = Set_Page_Start_Address_for_Page_Addressing_Mode(buffer_cmd[Counter], buffer_cmd_lenght, Cmd_START_PAGE_7);
//     Counter++;

//     for (uint8_t j = 0; j < Counter; j++)
//     {
//         for (uint8_t i = 0; i < cmd_lenght[j]; i++)
//         {
//             if (i == cmd_lenght[j]-1 && j == (Counter - 1))
//             {
//                 buffer_TWI[2 * (*buffer_TWI_lenght + i)] = 0x00;
//             }
//             else
//             {
//                 buffer_TWI[2 * (*buffer_TWI_lenght + i)] = 0x80;
//             }
//             buffer_TWI[2 * (*buffer_TWI_lenght + i) + 1] = buffer_cmd[j][i];
//         }
//         *buffer_TWI_lenght += cmd_lenght[j];
//     }

// }