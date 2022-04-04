
// -------------------------> Public functions <-------------------------------------------
// -------------------------> Private functions <-------------------------------------------
// -------------------------------> Macros <------------------------------------------------
#include <stm32f4xx.h>
#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

 __GPIOC_CLK_ENABLE();
 __GPIOD_CLK_ENABLE();
 __GPIOA_CLK_ENABLE();
 __GPIOB_CLK_ENABLE();


#define		LCD_BACKLIGHT_PORT		GPIOA
#define		LCD_BACKLIGHT_PORT_RCC()	__GPIOA_CLK_ENABLE();
#define		LCD_BACKLIGHT			GPIO_PIN_11

#define		LCD_CS_PORT				GPIOD
#define		LCD_CS_PORT_RCC()			 __GPIOD_CLK_ENABLE();
#define		LCD_CS					GPIO_PIN_7

#define		LCD_RES_PORT			GPIOA
#define		LCD_RES_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		LCD_RES					GPIO_PIN_12

#define		LCD_DCX_PORT			GPIOD
#define		LCD_DCX_PORT_RCC()		 __GPIOD_CLK_ENABLE();
#define		LCD_DCX					GPIO_PIN_13
#define		TOUCH_RESET_PORT		GPIOB
#define		TOUCH_RESET_PORT_RCC()	__GPIOB_CLK_ENABLE();
#define		TOUCH_RESET				GPIO_PIN_5


#define 	BUZZER_PORT				GPIOD
#define 	BUZZER_PORT_RCC()		__GPIOD_CLK_ENABLE();
#define 	BUZZER					GPIO_PIN_11

#define		BUTTON_0_PORT			GPIOA
#define		BUTTON_0_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		BUTTON_0				GPIO_PIN_8

#define		BUTTON_1_PORT			GPIOC
#define		BUTTON_1_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		BUTTON_1				GPIO_PIN_9

#define		RS485DIR_PORT			GPIOA
#define		RS485DIR_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		RS485DIR				GPIO_PIN_1

#define		ENCODER_PORT			GPIOC
#define		ENCODER_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		ENC_A					GPIO_PIN_7
#define		ENC_B					GPIO_PIN_6
#define		ENC_SW					GPIO_PIN_8

#define		LCD_BACKLIGHT_PORT		GPIOA
#define		LCD_BACKLIGHT_PORT_RCC()	 __GPIOA_CLK_ENABLE();
#define		LCD_BACKLIGHT			GPIO_PIN_11

#define		FLASH_WP_PORT			GPIOC
#define		FLASH_WP_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		FLASH_WP				GPIO_PIN_4

#define		FLASH_CS_PORT			GPIOA
#define		FLASH_CS_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		FLASH_CS				GPIO_PIN_4

#define		LCD_CS_PORT				GPIOD
#define		LCD_CS_PORT_RCC()			 __GPIOD_CLK_ENABLE();
#define		LCD_CS					GPIO_PIN_7

#define		LCD_RES_PORT			GPIOA
#define		LCD_RES_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		LCD_RES					GPIO_PIN_12

#define		LCD_DCX_PORT			GPIOD
#define		LCD_DCX_PORT_RCC()		 __GPIOD_CLK_ENABLE();
#define		LCD_DCX					GPIO_PIN_13

#define		LED_DBG_PORT			GPIOB
#define		LED_DBG_PORT_RCC()		 __GPIOB_CLK_ENABLE();
#define		LED_DBG_1				GPIO_PIN_3

#define		FRAM_WP_PORT			GPIOC
#define		FRAM_WP_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		FRAM_WP					GPIO_PIN_0

#define		FRAM_HOLD_PORT			GPIOC
#define		FRAM_HOLD_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		FRAM_HOLD				GPIO_PIN_1

#define		FRAM_CS_PORT			GPIOB
#define		FRAM_CS_PORT_RCC()		 __GPIOB_CLK_ENABLE();
#define		FRAM_CS					GPIO_PIN_9

#define		TOUCH_RESET_PORT		GPIOB
#define		TOUCH_RESET_PORT_RCC()	 __GPIOB_CLK_ENABLE();
#define		TOUCH_RESET				GPIO_PIN_5



#define 	ENC_FORWARD     0
#define 	ENC_BACKWARD    1

#define 	ENC_NOREADY     0
#define 	ENC_READY       1
#define 	ENC_INIT        3



void iopins_ini();
void encoder_ini(void);

void buzzer_on(void);
void buzzer_off(void);
void make_beep(void);

void rs485_trasmit_on(void);
void rs485_trasmit_off(void);


void get_encoder();

void ini_lcd_pwm(void);
void pwm_backlight_set(unsigned int pwm);

void interrupt_initialize_priorities();

typedef struct
{
	unsigned char value;
	unsigned char previous_value;

	unsigned char status;
	unsigned char direction;

} encoder_t;

extern encoder_t encoder;

extern unsigned long rtc;

void delay_ms(__IO uint32_t nCount1);
void delay_us(__IO uint32_t nCount1);

#endif /* LCD_DRIVER */

// -------------------------> Private variables <-------------------------------------------
// -------------------------> Public variables <--------------------------------------------
