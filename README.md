# JpgDecoder_STM
JPEG decoder for STM32 based on tjpgd project: http://elm-chan.org/fsw/tjpgd/00index.html

YouTube video:

https://youtu.be/kK9MdP6MZYc 

More ST7735 and STM32 projects:

https://www.youtube.com/watch?v=o3AqITHf0mo&list=PLxb1losWErZ6y6GombzvtwRZ2l7brPv1s

## Features:
- loading JPEG files from STM32 internal storage memory
- loading JPEG files from SD card via SdFat library
- The library requires about 3KB of RAM (can be shared for other applications)
- examples for ST7789 and ST7735
- slideshow examples: 25 images in 160x128 pixel resolution or 12 images in 240x240 pixel resolution stored in internal STM32 128KB!
- images can be rendered at given X,Y so it could be used for high quality user interface
- big images can be scaled down 1:2 to 1:8 and rendered directly on the LCD
- decoding and rendering 160x128 pixel JPEG takes about 80-100 ms, and 240x240 - about 220 ms

If you find it useful and you want to buy me a coffee or a beer:

https://www.paypal.me/cbm80amiga
