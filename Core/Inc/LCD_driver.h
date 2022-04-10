
// -------------------------> Public functions <-------------------------------------------
// -------------------------> Private functions <-------------------------------------------
// -------------------------------> Macros <------------------------------------------------
#include <stm32f4xx.h>
#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_


#define 	TFT_REG      			(*((volatile unsigned short *) 0x60000000))

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

//#define SPI_MASTER                   SPI1  define moved tro spi_routines

// <editor-fold desc="Colors">

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

//<editor-fold desc="Display commands">

/* Level 1 Commands */
#define LCD_SWRESET             0x01   /* Software Reset */
#define LCD_READ_DISPLAY_ID     0x04   /* Read display identification information */
#define LCD_RDDST               0x09   /* Read Display Status */
#define LCD_RDDPM               0x0A   /* Read Display Power Mode */
#define LCD_RDDMADCTL           0x0B   /* Read Display MADCTL */
#define LCD_RDDCOLMOD           0x0C   /* Read Display Pixel Format */
#define LCD_RDDIM               0x0D   /* Read Display Image Format */
#define LCD_RDDSM               0x0E   /* Read Display Signal Mode */
#define LCD_RDDSDR              0x0F   /* Read Display Self-Diagnostic Result */
#define LCD_SPLIN               0x10   /* Enter Sleep Mode */
#define LCD_SLEEP_OUT           0x11   /* Sleep out register */
#define LCD_PTLON               0x12   /* Partial Mode ON */
#define LCD_NORMAL_MODE_ON      0x13   /* Normal Display Mode ON */
#define LCD_DINVOFF             0x20   /* Display Inversion OFF */
#define LCD_DINVON              0x21   /* Display Inversion ON */
#define LCD_GAMMA               0x26   /* Gamma register */
#define LCD_DISPLAY_OFF         0x28   /* Display off register */
#define LCD_DISPLAY_ON          0x29   /* Display on register */
#define LCD_COLUMN_ADDR         0x2A   /* Colomn address register */
#define LCD_PAGE_ADDR           0x2B   /* Page address register */
#define LCD_GRAM                0x2C   /* GRAM register */
#define LCD_RGBSET              0x2D   /* Color SET */
#define LCD_RAMRD               0x2E   /* Memory Read */
#define LCD_PLTAR               0x30   /* Partial Area */
#define LCD_VSCRDEF             0x33   /* Vertical Scrolling Definition */
#define LCD_TEOFF               0x34   /* Tearing Effect Line OFF */
#define LCD_TEON                0x35   /* Tearing Effect Line ON */
#define LCD_MAC                 0x36   /* Memory Access Control register*/
#define LCD_VSCRSADD            0x37   /* Vertical Scrolling Start Address */
#define LCD_IDMOFF              0x38   /* Idle Mode OFF */
#define LCD_IDMON               0x39   /* Idle Mode ON */
#define LCD_PIXEL_FORMAT        0x3A   /* Pixel Format register */
#define LCD_WRITE_MEM_CONTINUE  0x3C   /* Write Memory Continue */
#define LCD_READ_MEM_CONTINUE   0x3E   /* Read Memory Continue */
#define LCD_SET_TEAR_SCANLINE   0x44   /* Set Tear Scanline */
#define LCD_GET_SCANLINE        0x45   /* Get Scanline */
#define LCD_WDB                 0x51   /* Write Brightness Display register */
#define LCD_RDDISBV             0x52   /* Read Display Brightness */
#define LCD_WCD                 0x53   /* Write Control Display register*/
#define LCD_RDCTRLD             0x54   /* Read CTRL Display */
#define LCD_WRCABC              0x55   /* Write Content Adaptive Brightness Control */
#define LCD_RDCABC              0x56   /* Read Content Adaptive Brightness Control */
#define LCD_WRITE_CABC          0x5E   /* Write CABC Minimum Brightness */
#define LCD_READ_CABC           0x5F   /* Read CABC Minimum Brightness */
#define LCD_READ_ID1            0xDA   /* Read ID1 */
#define LCD_READ_ID2            0xDB   /* Read ID2 */
#define LCD_READ_ID3            0xDC   /* Read ID3 */

/* Level 2 Commands */
#define LCD_RGB_INTERFACE       0xB0   /* RGB Interface Signal Control */
#define LCD_FRMCTR1             0xB1   /* Frame Rate Control (In Normal Mode) */
#define LCD_FRMCTR2             0xB2   /* Frame Rate Control (In Idle Mode) */
#define LCD_FRMCTR3             0xB3   /* Frame Rate Control (In Partial Mode) */
#define LCD_INVTR               0xB4   /* Display Inversion Control */
#define LCD_BPC                 0xB5   /* Blanking Porch Control register */
#define LCD_DFC                 0xB6   /* Display Function Control register */
#define LCD_ETMOD               0xB7   /* Entry Mode Set */
#define LCD_BACKLIGHT1          0xB8   /* Backlight Control 1 */
#define LCD_BACKLIGHT2          0xB9   /* Backlight Control 2 */
#define LCD_BACKLIGHT3          0xBA   /* Backlight Control 3 */
#define LCD_BACKLIGHT4          0xBB   /* Backlight Control 4 */
#define LCD_BACKLIGHT5          0xBC   /* Backlight Control 5 */
#define LCD_BACKLIGHT7          0xBE   /* Backlight Control 7 */
#define LCD_BACKLIGHT8          0xBF   /* Backlight Control 8 */
#define LCD_POWER1              0xC0   /* Power Control 1 register */
#define LCD_POWER2              0xC1   /* Power Control 2 register */
#define LCD_VCOM1               0xC5   /* VCOM Control 1 register */
#define LCD_VCOM2               0xC7   /* VCOM Control 2 register */
#define LCD_NVMWR               0xD0   /* NV Memory Write */
#define LCD_NVMPKEY             0xD1   /* NV Memory Protection Key */
#define LCD_RDNVM               0xD2   /* NV Memory Status Read */
#define LCD_READ_ID4            0xD3   /* Read ID4 */
#define LCD_PGAMMA              0xE0   /* Positive Gamma Correction register */
#define LCD_NGAMMA              0xE1   /* Negative Gamma Correction register */
#define LCD_DGAMCTRL1           0xE2   /* Digital Gamma Control 1 */
#define LCD_DGAMCTRL2           0xE3   /* Digital Gamma Control 2 */
#define LCD_INTERFACE           0xF6   /* Interface control register */

/* Extend register commands */
#define LCD_POWERA              0xCB   /* Power control A register */
#define LCD_POWERB              0xCF   /* Power control B register */
#define LCD_DTCA                0xE8   /* Driver timing control A */
#define LCD_DTCB                0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ           0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN           0xF2   /* 3 Gamma enable register */
#define LCD_PRC                 0xF7   /* Pump ratio control register */

//</editor-fold>

//<editor-fold desc="Orientation">

#define ORIENTATION_PORTRAIT 0x48
#define ORIENTATION_LANDSCAPE 0x28
#define ORIENTATION_PORTRAIT_MIRROR 0x88
#define ORIENTATION_LANDSCAPE_MIRROR 0xE8

//</editor-fold>

//<editor-fold desc="Dimensions">
#define LCD_PIXEL_WIDTH       480
#define LCD_PIXEL_HEIGHT      320
#define LCD_PIXEL_COUNT    LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT
//</editor-fold>

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
