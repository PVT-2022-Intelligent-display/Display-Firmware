
// -------------------------> Public functions <-------------------------------------------
// -------------------------> Private functions <-------------------------------------------
// -------------------------------> Macros <------------------------------------------------
#include <stm32f4xx.h>
#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#define 	MAX_PWM_PULSE  			65535

/*LCD_Colours*/
#define TRANSPARENT 			0x0001
#define BLACK           		0x0000      /*   0,   0,   0 */
#define NAVY            		0x000F      /*   0,   0, 128 */
#define DGREEN          		0x03E0      /*   0, 128,   0 */
#define DCYAN          			0x03EF      /*   0, 128, 128 */
#define MAROON          		0x7800      /* 128,   0,   0 */
#define PURPLE         		    0x780F      /* 128,   0, 128 */
#define OLIVE           	    0x7BE0      /* 128, 128,   0 */
#define LGRAY           	    0xC618      /* 192, 192, 192 */
#define DGRAY           	    0x7BEF      /* 128, 128, 128 */
#define BLUE            	    0x001F      /*   0,   0, 255 */
#define GREEN           	    0x07E0      /*   0, 255,   0 */
#define CYAN            	    0x07FF      /*   0, 255, 255 */
#define RED             	    0xF800      /* 255,   0,   0 */
#define MAGENTA					0xF81F      /* 255,   0, 255 */
#define YELLOW					0xFFE0      /* 255, 255,   0 */
#define WHITE					0xFFFF      /* 255, 255, 255 */
#define ORANGE					0xFD20      /* 255, 165,   0 */
#define GREENYELLOW				0xAFE5      /* 173, 255,  47 */
#define ORANGERED				0xEC0C		/* 236, 131, 99  */
#define KOVAR_RED			  	0xC023		/* 192, 5, 24  */
#define KOVAR_DARK_RED		  	0x9862		/* 157, 13, 22  */
#define BROWN                 	0XBC40 //
#define BRRED                 	0XFC07 //
/*LCD_ORIENTATION*/
#define ORIENTATION_PORTRAIT 0x48
#define ORIENTATION_LANDSCAPE 0x28
#define ORIENTATION_PORTRAIT_MIRROR 0x88
#define ORIENTATION_LANDSCAPE_MIRROR 0xE8
/*LCD_SIZE*/
#define LCD_PIXEL_WIDTH       480
#define LCD_PIXEL_HEIGHT      320
#define LCD_PIXEL_COUNT    LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT

extern unsigned long rtc;

void LCD_backlight_set(uint16_t pwm);
void Init_LCD();
void LCD_WritePixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_set_coordinates(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_fillRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t color);

#endif /* LCD_DRIVER */

