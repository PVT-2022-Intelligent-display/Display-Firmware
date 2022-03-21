#include "lcd_core.h"

static const uint8_t init_commands[] = {
  16, 0xE0, 0x00, 0x03, 0x09, 0x08, 0x16, 0x0A, 0x3F, 0x78, 0x4C, 0x09, 0x0A, 0x08, 0x16, 0x1A, 0x0F,
  16, 0xE1, 0x00, 0x16, 0x19, 0x03, 0x0F, 0x05, 0x32, 0x45, 0x46, 0x04, 0x0E, 0x0D, 0x35, 0x37, 0x0F,
  3, 0xC0, 0x17, 0x15,
  2, 0xC1,	0x41,    //VGH,VGL
  4, 0xC5,	0x00,	0x12,	0x80,
  2, LCD_MAC, 0x48,

  // *** INTERFACE PIXEL FORMAT: 0x66 -> 18 bit; 0x55 -> 16 bit
  2, LCD_PIXEL_FORMAT, 0x55,
  2, 0xB0, 0x00,    // 0x80: SDO NOT USE; 0x00 USE SDO
  2, 0xB1, 0xA0,    //60Hz
  2, 0xB4, 0x02,    //2-dot
  3, 0xB6, 0x02,  0x02,    //Source,Gate scan direction
  2, 0xE9, 0x00,    // Disable 24 bit data
  2, 0x53, 0x28,    // BCTRL && DD on
  2, 0x51, 0x7F,    //
  5, 0xF7,	0xA9,	0x51,	0x2C,	0x02,    // D7 stream, loose
  0
};


void lcd_configure(){

	uint8_t count;
	uint8_t *address = (uint8_t *) init_commands;

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, GPIO_PIN_RESET);
	    while (1) {
	        count = *(address++);
	        if (count-- == 0) break;
	        HAL_GPIO_WritePin(LCD_DCX_GPIO_Port,LCD_DCX_Pin, GPIO_PIN_RESET);
	        dmaSendCmdCont(*(address++));
	        HAL_GPIO_WritePin(LCD_DCX_GPIO_Port,LCD_DCX_Pin, GPIO_PIN_SET);
	        dmaSendDataCont8(address, count);
	        address += count;
	    }
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin, GPIO_PIN_SET);

}



