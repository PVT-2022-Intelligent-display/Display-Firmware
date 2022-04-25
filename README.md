
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

dataLen //number of hexadecimal bytes of additional data, see below. This value cannot go above 8192.

After those 6 numbers follow additional object data in hexadecimal format. The kind and ammount of theese data bytes is object-type dependant. This is where strings, colors etc. are provided. The data is send as hexadecimal characters (without the 0x prefix), and the last character is followed by newline. Make sure that you send the same ammount of bytes here as you specified in dataLen.

# Meanings of additional dataBytes

currently implemented:

## Rectangle
You should send 2 bytes of data, forming a single uint16_t number corresponding to the color of the rectangle. For color inspiration see /inc/LCD_driver.h

## Bitmap
You should send 2N bytes of data, where each 2 bytes form a uint16_t number corresponding to color of individual pixel, starting from upper left corner and going in rows.
Since dataLen is limited to 8192 bytes, bitmaps bigger than 64x64 pixels have to be split into multiple objects.

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
1
50
10
100
60
2
0000

```





