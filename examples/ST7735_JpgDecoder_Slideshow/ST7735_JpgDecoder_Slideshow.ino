// ST7735 library example
// 160x128 pixel JPEG Slideshow
// Requires Arduino_ST7735_STM and JpgDecoder_STM libraries and stm32duino
// (C)2019-20 Pawel A. Hernik
// YouTube video: https://youtu.be/kK9MdP6MZYc 

/*
 ST7735 128x160 1.8" LCD pinout (header at the top, from left):
 #1 LED   -> 3.3V
 #2 SCK   -> SCL/D13/PA5
 #3 SDA   -> MOSI/D11/PA7
 #4 A0/DC -> D8/PA1  or any digital
 #5 RESET -> D9/PA0  or any digital
 #6 CS    -> D10/PA2 or any digital
 #7 GND   -> GND
 #8 VCC   -> 3.3V
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#define TFT_CS  PA2
#define TFT_DC  PA1
#define TFT_RST PA0
#include <Arduino_ST7735_STM.h>
Arduino_ST7735 lcd = Arduino_ST7735(TFT_DC, TFT_RST, TFT_CS);

#include "logo.h"
#include "desert1.h"
#include "desert2.h"
#include "desert3.h"
#include "desert5.h"
#include "drawing.h"
#include "earth70.h"
#include "elcap.h"
#include "flower10.h"
#include "flower5.h"
#include "flower7.h"
#include "glassap.h"
#include "grnpalm.h"
#include "highsierra.h"
#include "inkcloud.h"
#include "kitten.h"
#include "lake30.h"
#include "landscape.h"
#include "mojave.h"
#include "phang.h"
#include "refl.h"
#include "poppies.h"
#include "yosemite50.h"
#include "sierra.h"
#include "sierra2.h"
#include "tropical.h"
#include "oranges.h"

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
  lcd.setRotation(3);
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
      case 0: jpeg.show( 0,  0, jpg, jpglen, scale); break;
      case 1: jpeg.show(80,  0, jpg, jpglen, scale); break;
      case 2: jpeg.show( 0, 64, jpg, jpglen, scale); break;
      case 3: jpeg.show(80, 64, jpg, jpglen, scale); break;
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
  //showJPG(refl,sizeof(refl));
  showJPG(lake30,sizeof(lake30));
  showJPG(tropical,sizeof(tropical));
  showJPG(logo,sizeof(logo));
  showJPG(desert1,sizeof(desert1));
  showJPG(flower5,sizeof(flower5));
  showJPG(desert2,sizeof(desert2));
  showJPG(sierra,sizeof(sierra));
  //showJPG(earth70,sizeof(earth70));
  showJPG(oranges,sizeof(oranges));
  showJPG(desert3,sizeof(desert3));
  showJPG(drawing,sizeof(drawing));
  showJPG(desert5,sizeof(desert5));
  showJPG(elcap,sizeof(elcap));
  showJPG(flower10,sizeof(flower10));
  showJPG(glassap,sizeof(glassap));
  showJPG(grnpalm,sizeof(grnpalm));
  showJPG(highsierra,sizeof(highsierra));
  showJPG(inkcloud,sizeof(inkcloud));
  showJPG(kitten,sizeof(kitten));
  showJPG(flower7,sizeof(flower7));
  showJPG(yosemite50,sizeof(yosemite50));
  showJPG(landscape,sizeof(landscape));
  showJPG(mojave,sizeof(mojave));
  showJPG(poppies,sizeof(poppies));
  showJPG(phang,sizeof(phang));
  showJPG(sierra2,sizeof(sierra2));
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

  del = 3000; scale = 1; thumb = 1; cnt = 0;
  slideshow();
}

