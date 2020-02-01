// ST7789 library example
// 240x240 pixel JPEG Slideshow
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

#include "beach.h"
#include "bg.h"
#include "burj.h"
#include "cat.h"
#include "colors.h"
#include "girls.h"
#include "goldengate.h"
#include "horses.h"
#include "mandy.h"
#include "planet.h"
#include "puppy.h"
#include "rabbit.h"
#include "toshiba.h"

// -------------------
#include <SdFat.h>
// undefine USE_SDFAT_LIBRARY in JpgDecoder_STM.h if SdFat is not used

#include <JpgDecoder_STM.h>

// below callback function renders decoded JPEG to LCD
bool renderToLCD(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  //lcdSPI();
  if(y>=lcd.height()) return 0; // 0 - to stop decoding
  lcd.drawImage(x,y, w,h, bitmap);
  return 1; // 1 - to decode next block
}

// align to a 32 bit boundary for the best performance
#define DECODE_BUFFER_LEN 3100 
uint8_t decodeBuffer[DECODE_BUFFER_LEN] __attribute__((aligned(4)));
JpgDecoder jpeg(renderToLCD,decodeBuffer,DECODE_BUFFER_LEN);
// -------------------

void setup()
{
  Serial.begin(115200);
  lcd.begin();
  lcd.fillScreen(BLACK);
  //lcd.setRotation(3);
}

char txt[50];
int del = 1000;
int scale = 0;
int thumb = 0;
int cnt = 0;

void showJPG(const unsigned char *jpg, int jpglen)
{
  uint16_t w,h;
  jpeg.getDim(&w, &h, jpg, jpglen);
  uint32_t t = millis();
  if(thumb) {
    switch(cnt) {
      case 0: jpeg.show(  0,   0, jpg, jpglen, scale); break;
      case 1: jpeg.show(120,   0, jpg, jpglen, scale); break;
      case 2: jpeg.show(  0, 120, jpg, jpglen, scale); break;
      case 3: jpeg.show(120, 120, jpg, jpglen, scale); break;
    }
  } else jpeg.show(0, 0, jpg, jpglen, scale);
  t = millis() - t;
  Serial.print(w);Serial.print(" x "); Serial.print(h);Serial.print(", len = ");Serial.print(jpglen);
  Serial.print(", time = ");Serial.print(t);Serial.println(" ms");
  if(del>0 && !thumb) delay(del);
  if(del>0 && thumb && cnt==3) delay(del);
  if(++cnt>3) cnt=0;
}

void slideshow()
{
  //showJPG(girls,sizeof(girls));
  showJPG(beach,sizeof(beach));
  showJPG(bg,sizeof(bg));
  showJPG(burj,sizeof(burj));
  showJPG(cat,sizeof(cat));
  showJPG(colors,sizeof(colors));
  showJPG(goldengate,sizeof(goldengate));
  showJPG(horses,sizeof(horses));
  showJPG(mandy,sizeof(mandy));
  showJPG(planet,sizeof(planet));
  showJPG(puppy,sizeof(puppy));
  showJPG(rabbit,sizeof(rabbit));
  showJPG(toshiba,sizeof(toshiba));
}

void loop()
{
  del = 2000; scale = 0; thumb = 0;
  slideshow();

  del = 0;
  slideshow();
  slideshow();
  slideshow();
  slideshow();

  del = 3000; scale = 1; thumb = 1;
  slideshow();
}

