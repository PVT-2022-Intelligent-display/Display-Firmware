




#include "TOUCH_driver.h"
#include "uart.h"
#include <stdio.h>
#include <stm32f4xx_hal_gpio.h>
#include "LCD_driver.h"
#include "configStructs.h"
#include "configLib.h"
#include "configStructs.h"
#include "objectVisualization.h"
#include "bitmapCacheLib.h"

extern int currentScreen;
extern int notYetDrawnFlag;

/*----------------------------------------------MACRO------------------------------------------------------------------*/
#if LINEAR_INTERPOLATION == 0
/*____________________TEXAS instruments method________________________*/
#define X_A     0.5229
#define X_B 	0.0028
#define X_L		-31.7532

#define Y_A		-0.0040
#define Y_B		0.3647
#define Y_L		-22.8412
/*____________________TEXAS instruments method END________________________*/
#endif

#define		MIN_X					0
#define		MAX_X					1024
#define		MIN_Y					0
#define		MAX_Y					1024

#define 	MAX_TP_BUTTONS			4
#define 	SLAVE_ADDRESS_TOUCH		0x90
#define 	I2C_DELAY				1000
#define 	LINEAR_INTERPOLATION    0

#define 	DEBUG_TOUCH             1  //DEBUG ENABLE WHEN 1

#define		TOUCH_RESET_PORT		GPIOB
#define		TOUCH_RESET_PORT_RCC()	__GPIOB_CLK_ENABLE();
#define		TOUCH_RESET				GPIO_PIN_5

/*----------------------------------------------Private variables define---------------------------------------------*/
I2C_HandleTypeDef hi2c1_touch;
touch_t touch;
struct element_t pressed_element;
uint8_t active_page;
uint8_t active_layer;
uint8_t slider_last_percents;
extern struct generalConfig gConf; //PB: Added extern because otherwise it does not work, you're not looking at the gConf from main but rather declaring a new one here.

/*----------------------------------------------Private function define----------------------------------------------*/
struct element_t touch_elements_register[MAX_NUM_PAGES][MAX_NUM_LAYERS][MAX_NUM_ELEMENTS];
//struct touch_t last_touch;
static unsigned short touch_read(unsigned char adr);
static void touch_write_control(unsigned char ctrl);
static void touch_write_register(uint8_t adr, uint16_t val);
static void delay_ms(__IO uint32_t nCount1);
static uint8_t validate_touch_coordinates(void);
static void touch_get_pressed_element();
#if LINEAR_INTERPOLATION == 1
	static long linear_interpolation(long x, long in_min, long in_max, long out_min, long out_max);
#endif

/*----------------------------------------------Public function-----------------------------------------------------*/
// Initialize touch screen
void set_page(uint8_t page){active_page = page;}
void set_layer(uint8_t layer){active_layer = layer;}
void touch_get_conf(struct generalConfig conf){gConf = conf;};
void touch_init(I2C_HandleTypeDef def)
{

	hi2c1_touch = def;
	touch_write_register(0x0E,0xC000);
	touch_write_control(0x80);
	for(int p = 0;p <MAX_NUM_PAGES; p++)
	{
		for(int l = 0;l <MAX_NUM_LAYERS; l++)
		{

			for(int i = 0;i <MAX_NUM_ELEMENTS; i++)
			{
				touch_elements_register[p][l][i].x = 0;
				touch_elements_register[p][l][i].x1 = 0;
				touch_elements_register[p][l][i].y = 0;
				touch_elements_register[p][l][i].y1 = 0;
				touch_elements_register[p][l][i].element_type = 0;
				touch_elements_register[p][l][i].isAlive = 0;
				touch_elements_register[p][l][i].element_data_pointer = 0;
				touch_elements_register[p][l][i].ID = 0;
			}
		}
	}
	pressed_element.x = 0;
	pressed_element.x1 = 0;
	pressed_element.y = 0;
	pressed_element.y1 = 0;
	pressed_element.element_type = 0;
	pressed_element.element_data_pointer = 0;
	pressed_element.isAlive = 0;
	pressed_element.ID = 0;
	active_layer = 0;
	active_page = 0;
	slider_last_percents = 0;
}
// Reset touch screen IC module
void touch_reset()
{
 	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_RESET);
 	delay_ms(10);
 	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_SET);
 	delay_ms(50);
}
// Return an structure type of element_t with last pressed btn data
struct element_t get_last_pressed_element()
{

	struct element_t pressed = pressed_element;

#if DEBUG_TOUCH == 1
	printf("LAST ELEMENT WAS %d %d\n\r", pressed_element.x,pressed_element.y);
	printf("SENDED %d %d\n\r", pressed.x,pressed.y);
#endif

	struct element_t empty;
	pressed_element = empty;
#if DEBUG_TOUCH == 1
	printf("RESETED TO %d %d\n\r", pressed_element.x,pressed_element.y);
#endif
	return pressed;
}
// After each call identifies if the touch was pressed and if TRUEm then identify the element
void touch_periodic_process()
{

	if(touch.pressed==1)
	{
		touch.pressed=0;

		touch.x=touch_read(0x00);
		touch.x1=touch_read(0x01);
		touch.y=touch_read(0x02);
		touch.y1=touch_read(0x03);
		touch.y=touch.x1;
		touch.x=1024-touch.y1;
		//last_touch.y = touch.y;
		if(validate_touch_coordinates()==1)
		{
#if DEBUG_TOUCH == 1
			printf("TOUCH = X: %d,Y: %d\n\r",touch.x,touch.y);
#endif
			 touch_get_pressed_element(touch);
		}
		touch_write_control(0x80);
	}
}
// External interrupt callback function. Take an action when touchscreen was pressed
static void touch_get_pressed_element()
{
	uint16_t x0,x1,y1,y0;
	for(int i = 0; i<MAX_NUM_ELEMENTS;i++)
	{

		if((touch_elements_register[active_page][active_layer][i].element_type != label )
			&& (touch_elements_register[active_page][active_layer][i].element_type != rectangle )
			&&(touch_elements_register[active_page][active_layer][i].element_type != picture )
			&&(touch_elements_register[active_page][active_layer][i].element_type != interactivelabel))
		{

			if((touch.x >= touch_elements_register[active_page][active_layer][i].x)
				&& (touch.x <= touch_elements_register[active_page][active_layer][i].x1)
				&&(touch.y >= touch_elements_register[active_page][active_layer][i].y)
				&&(touch.y <= touch_elements_register[active_page][active_layer][i].y1))
			{

			#if DEBUG_TOUCH == 1
						x0 = touch_elements_register[active_page][active_layer][i].x;
						x1 = touch_elements_register[active_page][active_layer][i].x1;
						y0 = touch_elements_register[active_page][active_layer][i].y;
						y1 = touch_elements_register[active_page][active_layer][i].y1;
						printf(" GETTING PRESSED ELEMENT \n\r X: %d\n\r X1: %d\n\r Y: %d\n\r Y1: %d\n\r",x0,x1,y0,y1);
			#endif
						memcpy(&pressed_element,&touch_elements_register[active_page][active_layer][i],sizeof(struct element_t));

			#if DEBUG_TOUCH == 1
						printf(" PRESSED ELEMENT \n\r X: %d\n\r X1: %d\n\r Y: %d\n\r Y1: %d\n\r TYPE: %d\n\r",pressed_element.x,pressed_element.x1,pressed_element.y,pressed_element.y1,pressed_element.element_type);

			#endif
						act_pressed_element();
			}
		}
	}


	//return touch_elements_register[-1];
}
void act_pressed_element()
{
	char reply[256];

#if DEBUG_TOUCH == 1
	printf("[TCH] Element type is: %d\n\r", pressed_element.element_type);
#endif
	if(pressed_element.element_type == screenbutton)
	{
		drawObjectToLcd(pressed_element.obj,pressed_element.element_data_pointer,1);
		delay_ms(50);
		drawObjectToLcd(pressed_element.obj,pressed_element.element_data_pointer,0);

		if (currentScreen < gConf.totalScreens - 1)
		{
			currentScreen++;
		}else
		{
			currentScreen = 0;
		}
		printf("Current screen , Total screens : %d %d \n\r", currentScreen, gConf.totalScreens);
		notYetDrawnFlag = 1;
		sprintf(reply,"Type:%d ID:%d Value:%d \n\r",pressed_element.element_type, pressed_element.ID,currentScreen);
#if DEBUG_TOUCH == 1
		printf("[TCH] Screenbutton was pressed\n\r");
		printf(reply);
#endif


		int ret = send_usart_message(reply, &huart2, strlen(reply));

	}else if(pressed_element.element_type == button)
	{
		sprintf(reply,"Type:%d ID:%d\n\r",pressed_element.element_type, pressed_element.ID,slider_last_percents);
#if DEBUG_TOUCH == 1
		printf("[TCH] Button was pressed\n\r");
		printf(reply);
#endif
		drawObjectToLcd(pressed_element.obj,pressed_element.element_data_pointer,1);
		delay_ms(50);
		drawObjectToLcd(pressed_element.obj,pressed_element.element_data_pointer,0);
		int ret = send_usart_message(reply, &huart2, strlen(reply));
	}else if(pressed_element.element_type == slider)
	{

		int slider_value = drawObjectToLcd(pressed_element.obj,pressed_element.element_data_pointer,touch.x);
		sprintf(reply,"Type:%d ID:%d Value: %d\n\r",pressed_element.element_type, pressed_element.ID,slider_value);
#if DEBUG_TOUCH == 1
		printf("[TCH] Slider was pressed\n\r");
		printf(reply);
#endif
		int ret = send_usart_message(reply, &huart2, strlen(reply));
	}
	else
	{

		sprintf(reply,"Type:%d ID:%d",pressed_element.element_type, pressed_element.ID);
#if DEBUG_TOUCH == 1
		printf(reply);
		printf("[TCH] Unknown element type %d\n\r", pressed_element.element_type);
#endif

		int ret = send_usart_message(reply, &huart2, strlen(reply));
	};
	//touch_reset();
}
// Register element to the touch elements array.
uint8_t touch_register_element(uint8_t page,uint8_t layer,struct object obj, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t element_type, uint32_t element_pointer,uint16_t ID)
{
	for(int i = 0;i<MAX_NUM_ELEMENTS;i++)
	{
		if(touch_elements_register[page][layer][i].isAlive == 0)
		{
			touch_elements_register[page][layer][i].obj = obj;
			touch_elements_register[page][layer][i].x = x0;
			touch_elements_register[page][layer][i].x1 =x1;
			touch_elements_register[page][layer][i].y =y0;
			touch_elements_register[page][layer][i].y1 =y1;
			touch_elements_register[page][layer][i].element_type = element_type;
			touch_elements_register[page][layer][i].element_data_pointer = element_pointer;
			touch_elements_register[page][layer][i].ID = ID;
			touch_elements_register[page][layer][i].isAlive = 1;
#if DEBUG_TOUCH == 1
			printf("\n\r [TCH] Element registered with index: %d\n\r on page: %d on layer: %d",i, active_page, active_layer);
			printf("\n\r [TCH] Element values: %d %d %d %d %d %d %d %d\n\r",x0,x1,y0,y1,element_pointer,element_type, ID);
#endif
			return 0;
		}
	}

	return -1;
}
//External interrupt from touc IC
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	touch.pressed=1;
#if DEBUG_TOUCH == 1
	printf("Interrupt touch");
#endif
}
//unregister element to be in touch elements queue
void touch_unregister_element(uint8_t page,uint8_t layer,uint8_t idx)
{
	touch_elements_register[page][layer][idx].x = 0;
	touch_elements_register[page][layer][idx].x1 = 0;
	touch_elements_register[page][layer][idx].y = 0;
	touch_elements_register[page][layer][idx].y1 = 0;
	touch_elements_register[page][layer][idx].element_type = 0;
	touch_elements_register[page][layer][idx].isAlive = 0;
	touch_elements_register[page][layer][idx].ID = 65535;
	touch_elements_register[page][layer][idx].element_data_pointer = 0;
}
/*----------------------------------------------Private function---------------------------------------------------*/
//Write to touch IC control command via I2C
static void touch_write_control(unsigned char ctrl)
{
	HAL_StatusTypeDef status =0;
	status = HAL_I2C_Master_Transmit(&hi2c1_touch, (uint16_t)SLAVE_ADDRESS_TOUCH, &ctrl, 1 ,I2C_DELAY);
#if DEBUG_TOUCH == 1
	printf("Write control I2C\n\r");
	printf ("%d",status);
#endif
}
//Write to touch IC data via I2C
static void touch_write_register(uint8_t adr, uint16_t val)
{

	HAL_StatusTypeDef status =0;
	uint8_t data[2];
	data[0] = val>>8;
	data[1] = (uint8_t)val;
	status = HAL_I2C_Mem_Write(&hi2c1_touch, (uint16_t)SLAVE_ADDRESS_TOUCH,(adr<<3),I2C_MEMADD_SIZE_8BIT,data,2,I2C_DELAY);
#if DEBUG_TOUCH == 1
	printf ("I2C write register\n\r");
	printf ("status %d \n\r",status);
#endif
}
//Read data via I2C from touch IC
static unsigned short touch_read(unsigned char adr)
{
	unsigned char adr_i2c= 0x01;
	adr_i2c+=adr<<3;
	uint8_t data[2];
	HAL_StatusTypeDef status =0;
	status = HAL_I2C_Mem_Read(&hi2c1_touch, (uint16_t)SLAVE_ADDRESS_TOUCH ,adr_i2c,I2C_MEMADD_SIZE_8BIT,data, 2, I2C_DELAY);
	uint16_t result = (data[0] << 8) | (data[1] & 0xFF);
	return result;
}
//Transform and validate touch coordinates
static uint8_t validate_touch_coordinates(void)
{
	if(touch.x<MIN_X) return 0;
	if(touch.x>MAX_X) return 0;
	if(touch.y<MIN_Y) return 0;
	if(touch.y>MAX_Y) return 0;

	touch.x=touch.x - MIN_X;
	touch.y=touch.y - MIN_Y;

#if LINEAR_INTERPOLATION == 1
	touch.x = linear_interpolation((long)touch.x,40,970,0,480);
	touch.y = linear_interpolation((long)touch.y,40,970,0,340);
#else
	double x,y;
	x = X_A* (int16_t)touch.x + X_B *(int16_t)touch.y +X_L;
	y = Y_A* (int16_t)touch.x + Y_B *(int16_t)touch.y +Y_L;
	touch.x = (uint16_t)x;
	touch.y = (uint16_t)y;
#endif
	if (touch.x >= 0 && touch.x <= LCD_PIXEL_WIDTH)
	{
		return 1;
	}
	if (touch.y >= 0 && touch.y <= LCD_PIXEL_HEIGHT)
	{
		return 1;
	}


	return 0;
}
//Linear interpolation function
#if LINEAR_INTERPOLATION == 1
	static long linear_interpolation(long x, long in_min, long in_max, long out_min, long out_max)
	{
		  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
#endif
//Delay in ms
static void delay_ms(__IO uint32_t nCount1) {
	__IO uint32_t nCount=nCount1*23666;
	while(nCount--) {
  }
}
