
# Configuration protocol:

(all words shown here in "quotes" are sent without quotes)

Each configuration begins with "config" followed by newline. Then, number of screens is sent as decimal number in plaintext, followed by newline.

After this, configuration of each screen is sent. Each screen configuration begins with the word "screen" followed by newline. After that, the number of objects contained in this screen is sent as decimal number in plaintext, followed by newline.

After that, objects of current screen are sent. The order in which the objects are sent determines the order of their rendering; so if one object is layered over another, the bottom one should be sent first.

Each object begins with it's identifier, followed by newline. Identifiers implemented at time of writing are "rectangle", "button", "label", "bitmap". (see src/configLib.c, variable typeNames[] for complete list).

After the identifier line, 6 decimal numbers are sent in plaintext, each followed by a newline. The numbers are stored as uint16_t in the stm, so please don't send millions or negative numbers.

The numbers are, in order:
id //unique identifier of object

xstart //x coordinate of upper left corner of object

ystart //y -||-

xend   //x coordinate of lower right corner of object

yend   //y -||-

dataLen //number of bytes of additional data, see below. This value cannot go above 8192.

After those 6 numbers follow additional object data in hexadecimal format. The kind and ammount of theese data bytes is object-type dependant. This is where strings, colors etc. are provided. The data is send as hexadecimal characters (without the 0x prefix), and the last character is followed by newline. Make sure that you send the same ammount of bytes here as you specified in dataLen.

# Meanings of additional dataBytes

currently implemented:

## Rectangle
You should send 2 bytes of data, forming a single uint16_t number corresponding to the color of the rectangle. For color inspiration see /inc/LCD_driver.h

## Bitmap (outdated, to be replaced)
You should send 2N bytes of data, where each 2 bytes form a uint16_t number corresponding to color of individual pixel, starting from upper left corner and going in rows.
Since dataLen is limited to 8192 bytes, bitmaps bigger than 64x64 pixels have to be split into multiple objects.

## Label
You should send N+8 bytes of data, where n is the number of characters in your label as expanded ascii bytes. The first 8 bytes descripbe the text and their meanings are as follows:

byte 1 - pixel scaling. Defines how many LCD pixels are drawn per pixel of font. Font is 5x7, so when pixelscaling = 5, a single character will take up 25x35 pixels on lcd.

byte 2 - horizontal spacing. How many lcd pixels between chars in x axis.

byte 3 - vertical spacing. How many lcd pixels between chars in y axis (useful when you send newlines in your string)

byte 4 - use background. When 0, the pixels around characters will be left as they are (transparent background). When not 0, they will be filled with backgroundColor

bytes 5,6 - uint16_t color of text

bytes 7,8 - uint16_t color of background. Has no effect when byte 4 = 0 (but still has to be sent).

Label example: To draw "HELLO" in black with transparent background, pixel scaling 3, horizontal and vertical spacing 1 pixel, send following bytes as data: [030101000000000048454c4c4f]


# Communication example

Communication example configuring 2 screens, where first screen contains a red rectangle and a 2x2 bitmap and second screen contains a black square.

Text after // are comments, only text before // is sent.

```
config
2 //number of screens
screen
2 //number of objects in first screen
rectangle
0 //id
0 //xstart
0 //ystart
150 //xend
20 //yend
2 //dataLen - 2 bytes
f800 //color red
bitmap
1
10
10
12
12
8
f8000000f8000000 //this bitmap contains 2 red and 2 black pixels
screen
1 //number of objects in 2nd screen
rectangle
2
50
10
100
60
2
0000

```

# Adding elements to be detected by TOUCH_driver module

To add any element to be detected by touch screen, use touch_register_element function. Each ellement can be placed on one of 5 pages, each page has 5 layers and moreover, each layer has 20 ellements (exact limits can be changed in TOUCH_screen module). 
```C
uint8_t touch_register_element(uint8_t page,uint8_t layer,uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t element_type, uint32_t element_pointer);
```
EXAMPLE:
```C	
LCD_fillRect(256,154,50,50,RED); // ---> will draw red rectagle in the middle of the sreen
touch_register_element(0,0,256,154,256+50,154+50,0,0)); // ----> will add coodinates of the red rectangle to be recognized as a ellement 
```
Red rectangle which will be placed in the middle of the screen will be recognized by TOUCh driver module as an element on page 0 and layer 0. Index on the layer will be set automatically and will be a function return value. In each itteration of main loop touch_pereodic_process must be called to check while any buttons were pressed. After touch_pereodic_process was called you can call get_last_pressed_element function, which will return structure type of element_t that will contain information about last pressed UI element that was previosly specified in the touch_register_element function


