// ST7789 library example
// JPEG Decoder and Viewer (from storage memory)
// Requires Arduino_ST7789_STM and JpgDecoder_STM libraries and stm32duino
// (C)2019-20 Pawel A. Hernik
// YouTube video: https://youtu.be/kK9MdP6MZYc 

/*
 ST7789 240x240 1.3" IPS (without CS pin) connections (only 6 wires required):

 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> SCK/D13/PA5
 #04 SDA -> MOSI/D11/PA7
 #05 RES -> D8/PA0 or any digital
 #06 DC  -> D7/PA1 or any digital
 #07 BLK -> NC
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#define TFT_DC    PA1
#define TFT_RST   PA0
#include <Arduino_ST7789_STM.h>
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

#include "oranges.h"
#include "earth.h"

// -------------------
#include <SdFat.h>
// undefine USE_SDFAT_LIBRARY in JpgDecoder_STM.h if SdFat is not used

#include <JpgDecoder_STM.h>

// the callback function below renders decoded JPEG on the LCD
bool renderLCD(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  //lcdSPI();
  if(y>=lcd.height()) return 0; // 0 - to stop decoding
  lcd.drawImage(x,y, w,h, bitmap);
  return 1; // 1 - to decode next block
}

// align to a 32 bit boundary for the best performance
#define DECODE_BUFFER_LEN 3100 
uint8_t decodeBuffer[DECODE_BUFFER_LEN] __attribute__((aligned(4)));
JpgDecoder jpeg(renderLCD,decodeBuffer,DECODE_BUFFER_LEN);

// -------------------

void setup()
{
  Serial.begin(115200);
  lcd.begin();
  lcd.fillScreen(BLACK);
}

void loop()
{
  jpeg.show(0,0, oranges,sizeof(oranges));
  delay(2000);
  jpeg.show(0,0, earth,sizeof(earth));
  delay(2000);
  jpeg.show(0,0, oranges,sizeof(oranges), 1);
  jpeg.show(130,0, oranges,sizeof(oranges), 2);
  jpeg.show(210,0, oranges,sizeof(oranges), 3);
  delay(3000);
}

