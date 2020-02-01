// ST7735 library example
// JPEG Decoder and Viewer (from SD card)
// Requires Arduino_ST7735_STM, SdFat and JpgDecoder_STM libraries and stm32duino
// (C)2019-20 Pawel A. Hernik

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

// -----------------------------------------------
// Required for JPEG loading from SD card and rendering
#include "SdFat.h"
// define USE_SDFAT_LIBRARY in JpgDecoder_STM.h too

// use 18 if your SD card doesn't work
#define SD_SPEED 36
//#define SD_SPEED 18
#define USE_SDIO 0
const uint8_t SD_CS = PA4;
SdFat sd(1);

#include <JpgDecoder_STM.h>

// use 18 if your SD card doesn't work
#define SD_SPEED 36
//#define SD_SPEED 18
void sdSPI() { SPI.beginTransaction(SD_SCK_MHZ(SD_SPEED)); }
void lcdSPI() { SPI.beginTransaction(SPISettings(36000000, MSBFIRST, SPI_MODE3, DATA_SIZE_16BIT)); }

// the callback function below renders decoded JPEG on the LCD
bool renderLCD(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  lcdSPI();
  if(y>=lcd.height()) return 0; // 0 - to stop decoding
  lcd.drawImage(x,y, w,h, bitmap);
  return 1; // 1 - to decode next block
}

// align to a 32 bit boundary for the best performance
#define DECODE_BUFFER_LEN 3100 
uint8_t decodeBuffer[DECODE_BUFFER_LEN] __attribute__((aligned(4)));
JpgDecoder jpeg(renderLCD,decodeBuffer,DECODE_BUFFER_LEN);
// -----------------------------------------------

void setup()
{
  Serial.begin(115200);
  lcd.begin();
  if(!sd.cardBegin(SD_CS, SD_SCK_MHZ(SD_SPEED))) {
    lcdSPI(); lcd.fillScreen(RED);
    Serial.println("\nSD Card\ninitialization\nfailed.\n");
    while(1);
  }
  if(!sd.fsBegin()) {
    lcdSPI(); lcd.fillScreen(YELLOW);
    Serial.println("\nFile System\ninitialization\nfailed.\n");
    while(1);
  }
  //sd.chdir("stm32");
  //sd.ls("stm32", 0); 

  lcdSPI();
  lcd.fillScreen(BLACK);
  lcd.setRotation(3);
}

void loop()
{
  jpeg.show(0,0,"/stm32/JPEGs/Yosemite5.jpg");
  delay(2000);
  jpeg.show(16,0,"/stm32/CubosColores.jpg");
  delay(2000);
  jpeg.show(0,0,"/stm32/JPEGs/Poppies.jpg");
  delay(2000);
  jpeg.show(  0,0,"/stm32/CubosColores.jpg",1);
  jpeg.show( 90,0,"/stm32/CubosColores.jpg",2);
  jpeg.show(140,0,"/stm32/CubosColores.jpg",3);
  delay(2000);
}
