/*
 * SSD1306_cmd.c
 *
 * Created: 28/08/2025 12:02:33 a. m.
 *  Author: Marquez_diaz
 */ 

 #include "Driver\Display\OLED\SSD1306\SSD1306_cmd.h"

 
// ----------------------------  Fundamental ----------------------------------------
u_int8_t Set_Contrast_Control(u_int8_t *buf, u_int8_t buf_size,u_int8_t contrast_value)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0x81; // Set Contrast Control Command
	buf[1] = contrast_value; // Contrast value

	return needed; // return the number of bytes written
}

uint8_t Entire_Display_ON(u_int8_t *buf, u_int8_t buf_size, bool resume_0_ignore_1)
{
	const size_t needed = 1;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = resume_0_ignore_1 ? 0xA5 : 0xA4; // Set to 1 for ON, 0 for OFF

	return needed; // return the number of bytes written
}

uint8_t Set_N_I_Display(u_int8_t *buf, u_int8_t buf_size, bool display_normal_0_inverse_1)
{
	const size_t needed = 1;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = display_normal_0_inverse_1 ? 0xA7 : 0xA6; // Set Normal or Inverse Display

	return needed; // return the number of bytes written
}

uint8_t Set_Display_ON_OFF(u_int8_t *buf, u_int8_t buf_size, bool display_off_0_on_1)
{
	const size_t needed = 1;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = display_off_0_on_1 ? 0xAF : 0xAE; // Set Display ON or OFF

	return needed; // return the number of bytes written
}



// ----------------------------  Set Scrolling Command ----------------------------------------

u_int8_t Continuous_Horizontal_Scroll(u_int8_t *buf, u_int8_t buf_size,
                                    bool Right_0_Left_1_Horizontal_Scroll,
                                    Cmd_START_PAGE start_page,
                                    Cmd_SET_FRAME set_frame,
                                    Cmd_FINISH_PAGE finish_page)
{
	const size_t needed = 7;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = Right_0_Left_1_Horizontal_Scroll ? 0x26 : 0x27; // Right or Left Horizontal Scroll Command
	buf[1] = 0x00; // dummy
	buf[2] = start_page; // start page
	buf[3] = set_frame;
	buf[4] = finish_page;
	buf[5] = 0x00; // dummy
	buf[6] = 0xFF; // dummy

	return needed; // return the number of bytes written
}

u_int8_t Continuous_Vertical_and_Horizontal_Scroll(u_int8_t *buf, u_int8_t buf_size,
                                                bool Right_0_Left_1_Horizontal_and_vertical_Scroll,
                                                Cmd_START_PAGE start_page,
                                                Cmd_SET_FRAME set_frame,
                                                Cmd_FINISH_PAGE finish_page,
                                                u_int8_t scrolling_offset)
{
	const size_t needed = 7;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = Right_0_Left_1_Horizontal_and_vertical_Scroll ? 0x29 : 0x2A; // Right or Left Horizontal Scroll Command
	buf[1] = 0x00; // dummy
	buf[2] = start_page; // start page
	buf[3] = set_frame;
	buf[4] = finish_page;
	buf[5] = 0x00; // dummy
	buf[6] = 0xFF; // dummy

	return needed; // return the number of bytes written
}

u_int8_t Deactivate_Scroll(u_int8_t *buf, u_int8_t buf_size)
{
	const size_t needed = 1;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0x2E; // Deactivate Scroll Command

	return needed; // return the number of bytes written
}

u_int8_t Activate_Scroll(u_int8_t *buf, u_int8_t buf_size)
{
	const size_t needed = 1;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0x2F; // Activate Scroll Command

	return needed; // return the number of bytes written
}

u_int8_t Set_Vertical_Scroll_Area(u_int8_t *buf, u_int8_t buf_size,
                                u_int8_t No_rows_fixed_area,
                                u_int8_t No_rows_scroll_area)
{
	const size_t needed = 3;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0xA3; // Set Vertical Scroll Area Command
	buf[1] = No_rows_fixed_area; // Number of rows in fixed area
	buf[2] = No_rows_scroll_area; // Number of rows in scroll area

	return needed; // return the number of bytes written
}

// ----------------------------  Addressing Setting command ----------------------------------------

u_int8_t Set_Lower_Column_Start_Address_for_Page_Addressing_Mode(u_int8_t *buf, u_int8_t buf_size,
                                                                u_int8_t lower_column_address)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = lower_column_address; // Lower Column Start Address Command

	return needed; // return the number of bytes written
}

u_int8_t Set_High_Column_Start_Address_for_Page_Addressing_Mode(u_int8_t *buf, u_int8_t buf_size,
                                                                u_int8_t high_column_address)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = (high_column_address | 0x10); // Higher Column Start Address Command

	return needed; // return the number of bytes written
}

u_int8_t Set_Memory_Addressing_Mode(u_int8_t *buf, u_int8_t buf_size,
                                Cmd_MEMORY_MODE addressing_mode)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0x20; // Set Memory Addressing Mode Command
	buf[1] = addressing_mode; // Memory Addressing Mode

	return needed; // return the number of bytes written
}

u_int8_t Set_Column_Address(u_int8_t *buf, u_int8_t buf_size,
						u_int8_t start_column_address,
						u_int8_t end_column_address)
{
	const size_t needed = 3;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0x21; // Set Column Address Command
	buf[1] = start_column_address; // Start Column Address
	buf[2] = end_column_address; // End Column Address

	return needed; // return the number of bytes written
}

u_int8_t Set_Page_Address(u_int8_t *buf, u_int8_t buf_size,
						Cmd_START_PAGE start_page_address,
						Cmd_FINISH_PAGE end_page_address)
{
	const size_t needed = 3;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0x22; // Set Page Address Command
	buf[1] = start_page_address; // Start Page Address
	buf[2] = end_page_address; // End Page Address

	return needed; // return the number of bytes written
}

u_int8_t Set_Page_Start_Address_for_Page_Addressing_Mode(u_int8_t *buf, u_int8_t buf_size,
								Cmd_START_PAGE page_start_address)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0xB0 | page_start_address; // Set Page Start Address Command

	return needed; // return the number of bytes written
}

// ----------------------------  Hardware Configuracion Command ----------------------------------------

u_int8_t Set_Display_Start_Line(u_int8_t *buf, u_int8_t buf_size,
                                u_int8_t display_start_line)
{
	const size_t needed = 1;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0x40 | display_start_line; // Set Display Start Line Command
	return needed; // return the number of bytes written
}	

u_int8_t Set_Segment_Remap(u_int8_t *buf, u_int8_t buf_size,
                                bool segment_re_map)
{
	const size_t needed = 1;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = segment_re_map ? 0xA1 : 0xA0; // Set Segment Re-map Command

	return needed; // return the number of bytes written
}								

/*
	hohh
*/
u_int8_t Set_Multiplex_Ratio(u_int8_t *buf, u_int8_t buf_size,
                                u_int8_t multiplex_ratio)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0xA8; // Set Multiplex Ratio Command
	buf[1] = multiplex_ratio; // Multiplex Ratio Value (1 to 64)

	return needed; // return the number of bytes written
}		

u_int8_t Set_COM_Output_Scan_Direction(u_int8_t *buf, u_int8_t buf_size, bool mode_normal_0_remapped_1) 
{
	const size_t needed = 1;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = mode_normal_0_remapped_1 ? 0xC8 : 0xC0; // Set COM Output Scan Direction Command

	return needed; // return the number of bytes written
}

u_int8_t Set_Display_Offset(u_int8_t *buf, u_int8_t buf_size, 
                                u_int8_t display_offset_value)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0xD3; // Set Display Offset Command
	buf[1] = display_offset_value; // Display Offset Value (0 to 63)

	return needed; // return the number of bytes written
}		

u_int8_t Set_COM_Pins_Hardware_Configuration(u_int8_t *buf, u_int8_t buf_size,
								bool Seq_Alt_COM, bool Enable_Disable_COM)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0xDA; // Set COM Pins Hardware Configuration Command
	buf[1] = 0x02 | (Seq_Alt_COM << 4) | (Enable_Disable_COM << 5); // COM Pins Configuration

	return needed; // return the number of bytes written
}

// ----------------------------  Timing and Driving Scheme Setting Command ----------------------------------------

u_int8_t Set_Display_Clock_Divide_Ratio_and_OSC_Frequency(u_int8_t *buf, u_int8_t buf_size,
															u_int8_t divide_ratio, u_int8_t osc_frequency)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0xD5; // Set Display Clock Divide Ratio and OSC Frequency Command
	buf[1] = (osc_frequency << 4) | divide_ratio; // Divide Ratio and OSC Frequency

	return needed; // return the number of bytes written
}

u_int8_t Set_Precharge_Period(u_int8_t *buf, u_int8_t buf_size,
								u_int8_t precharge_period_phase_1,
								u_int8_t precharge_period_phase_2)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0xD9; // Set Precharge Period Command
	buf[1] = (precharge_period_phase_1 << 4) | precharge_period_phase_2; // Precharge Period Phase 1 and Phase 2
	

	return needed; // return the number of bytes written
}

u_int8_t Charge_Pump_Command(u_int8_t *buf, u_int8_t buf_size,
							bool Enable_1_Disable_0_Charge_Pump)
{
	const size_t needed = 2;
	if (!buf || buf_size < needed)
		return 0;

	buf[0] = 0x8D; // Charge Pump Setting Command
	buf[1] = Enable_1_Disable_0_Charge_Pump ? 0x14 : 0x10; // Enable or Disable Charge Pump

	return needed; // return the number of bytes written
}

// void TWI_custom_write(uint8_t *data, uint32_t lenght_data)
// {
//    twi_packet_t pkt = {
//  		.chip        = 0x3D,
//  		.buffer      = (void *)data,
//  		.length      = lenght_data
//  	};

//     twi_master_write(TWI0, &pkt);
// }

// void configure_pins_TWI_custom(void)
// {
//     // Habilitar el reloj para el PIOB
//     pmc_enable_periph_clk(ID_TWI0);
//     pmc_enable_periph_clk(ID_PIOA);

//     // salida
//     pio_configure(PIOA, PIO_PERIPH_A, PIO_PA18 | PIO_PA17, PIO_DEFAULT);

//     twi_options_t opts = {
//         .speed = 100000,
//         .smbus = 0, // i2c clasico
//         .master_clk = sysclk_get_cpu_hz()
//     };

//     twi_master_init(TWI0,&opts);
// }
