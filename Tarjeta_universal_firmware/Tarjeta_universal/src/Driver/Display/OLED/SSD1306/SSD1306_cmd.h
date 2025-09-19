/*
 * SSD1306_cmd.h
 *
 * Created: 28/08/2025 12:01:56 a. m.
 *  Author: Marquez_diaz
 */ 


#ifndef SSD1306_CMD_H_
#define SSD1306_CMD_H_


#include <asf.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>

// ----------------- user -------------------------------


// ---------------------------  Fundamental -------------------
u_int8_t Set_Contrast_Control(u_int8_t *buf, u_int8_t buf_size,u_int8_t contrast_value);
u_int8_t Entire_Display_ON(u_int8_t *buf, u_int8_t buf_size, bool resume_0_ignore_1);
u_int8_t Set_N_I_Display(u_int8_t *buf, u_int8_t buf_size, bool display_normal_0_inverse_1);
u_int8_t Set_Display_ON_OFF(u_int8_t *buf, u_int8_t buf_size, bool display_off_0_on_1);

// ---------------Set_Scrolling_Command -------------------

typedef enum
{
    Cmd_START_PAGE_0,
    Cmd_START_PAGE_1,
    Cmd_START_PAGE_2,
    Cmd_START_PAGE_3,
    Cmd_START_PAGE_4,
    Cmd_START_PAGE_5,
    Cmd_START_PAGE_6,
    Cmd_START_PAGE_7
} Cmd_START_PAGE;

typedef enum
{
    Cmd_SET_FRAME_5   = 0x00,
    Cmd_SET_FRAME_64  = 0x01,
    Cmd_SET_FRAME_128 = 0x02,
    Cmd_SET_FRAME_256 = 0x03,
    Cmd_SET_FRAME_3   = 0x04,
    Cmd_SET_FRAME_4   = 0x05,
    Cmd_SET_FRAME_25  = 0x06,
    Cmd_SET_FRAME_2   = 0x07
} Cmd_SET_FRAME;

typedef enum
{
    Cmd_FINISH_PAGE_0,
    Cmd_FINISH_PAGE_1,
    Cmd_FINISH_PAGE_2,
    Cmd_FINISH_PAGE_3,
    Cmd_FINISH_PAGE_4,
    Cmd_FINISH_PAGE_5,
    Cmd_FINISH_PAGE_6,
    Cmd_FINISH_PAGE_7
} Cmd_FINISH_PAGE;

u_int8_t Continuous_Horizontal_Scroll(u_int8_t *buf, u_int8_t buf_size,
                                    bool Right_0_Left_1_Horizontal_Scroll,
                                    Cmd_START_PAGE start_page,
                                    Cmd_SET_FRAME set_frame,
                                    Cmd_FINISH_PAGE finish_page);

u_int8_t Continuous_Vertical_and_Horizontal_Scroll(u_int8_t *buf, u_int8_t buf_size,
                                                bool Right_0_Left_1_Horizontal_and_vertical_Scroll,
                                                Cmd_START_PAGE start_page,
                                                Cmd_SET_FRAME set_frame,
                                                Cmd_FINISH_PAGE finish_page,
                                                u_int8_t scrolling_offset);

u_int8_t Deactivate_Scroll(u_int8_t *buf, u_int8_t buf_size);

u_int8_t Activate_Scroll(u_int8_t *buf, u_int8_t buf_size);

u_int8_t Set_Vertical_Scroll_Area(u_int8_t *buf, u_int8_t buf_size,
                                u_int8_t No_rows_fixed_area,
                                u_int8_t No_rows_scroll_area);
                         
// ----------------------------  Addressing Setting command ----------------------------------------



u_int8_t Set_Lower_Column_Start_Address_for_Page_Addressing_Mode(u_int8_t *buf, u_int8_t buf_size,
                                                                u_int8_t lower_column_address);
u_int8_t Set_High_Column_Start_Address_for_Page_Addressing_Mode(u_int8_t *buf, u_int8_t buf_size,
                                                                u_int8_t high_column_address);
typedef enum
{
    Cmd_MEMORY_MODE_HORIZONTAL = 0x00,
    Cmd_MEMORY_MODE_VERTICAL = 0x01,
    Cmd_MEMORY_MODE_PAGE = 0x02
} Cmd_MEMORY_MODE;

u_int8_t Set_Memory_Addressing_Mode(u_int8_t *buf, u_int8_t buf_size,
                                Cmd_MEMORY_MODE addressing_mode);
u_int8_t Set_Column_Address(u_int8_t *buf, u_int8_t buf_size,
                        u_int8_t start_column_address,
                        u_int8_t end_column_address);
u_int8_t Set_Page_Address(u_int8_t *buf, u_int8_t buf_size,
                        Cmd_START_PAGE start_page_address,
                        Cmd_FINISH_PAGE end_page_address);
u_int8_t Set_Page_Start_Address_for_Page_Addressing_Mode(u_int8_t *buf, u_int8_t buf_size,
                                Cmd_START_PAGE page_start_address);                        

// ----------------------------  Hardware Configuracion Command ----------------------------------------                                

u_int8_t Set_Display_Start_Line(u_int8_t *buf, u_int8_t buf_size,
                                u_int8_t display_start_line);
u_int8_t Set_Segment_Remap(u_int8_t *buf, u_int8_t buf_size,
                                bool segment_re_map);
u_int8_t Set_Multiplex_Ratio(u_int8_t *buf, u_int8_t buf_size,
                                u_int8_t multiplex_ratio);
u_int8_t Set_COM_Output_Scan_Direction(u_int8_t *buf, u_int8_t buf_size, bool mode_normal_0_remapped_1);

u_int8_t Set_Display_Offset(u_int8_t *buf, u_int8_t buf_size, 
                                u_int8_t display_offset_value);

u_int8_t Set_COM_Pins_Hardware_Configuration(u_int8_t *buf, u_int8_t buf_size,
								bool Seq_Alt_COM, bool Enable_Disable_COM);

// ----------------------------  Timing and Driving Scheme Setting Command ----------------------------------------
u_int8_t Set_Display_Clock_Divide_Ratio_and_OSC_Frequency(u_int8_t *buf, u_int8_t buf_size,
                                u_int8_t divide_ratio,
                                u_int8_t osc_frequency);
u_int8_t Set_Precharge_Period(u_int8_t *buf, u_int8_t buf_size,
                                u_int8_t precharge_period_phase_1,
                                u_int8_t precharge_period_phase_2);   


u_int8_t Charge_Pump_Command(u_int8_t *buf, u_int8_t buf_size,
							bool Enable_1_Disable_0_Charge_Pump);  


#endif /* SSD1306_CMD_H_ */