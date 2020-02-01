// JPEG Decoder for STM32 using TJpgDec
// (c) 2019/20 by Pawel A. Hernik
// Based on Bodmer's code and TJpgDec project:
// http://elm-chan.org/fsw/tjpgd/00index.html

#ifndef JpgDecoder_STM_H
#define JpgDecoder_STM_H

#define USE_SDFAT_LIBRARY

// Use PROGMEM for tables, saves 1K RAM when JD_TBLCLIP is set to 1 in tjpgd.h
// #define TJPG_USE_PROGMEM

#include "Arduino.h"
#include "tjpgd.h"

#if defined USE_SDFAT_LIBRARY
#include "SdFat.h"
#endif

//------------------------------------------------------------------------------

class JpgDecoder {

private:
#if defined USE_SDFAT_LIBRARY
  static SdFile jpgSdFile;
#endif
  static const uint8_t *data;
  static uint32_t dataIdx;
  static uint32_t dataLen;
  static int16_t startX;
  static int16_t startY;
  static bool (*renderFun)(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *data);
  uint8_t *decodeBuffer;
  uint16_t decodeBufferLen;

public:
  JpgDecoder(bool (*fun)(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *data), uint8_t *buf, uint16_t len);

  static uint16_t renderLCD(JDEC *jdec, void *bitmap, JRECT *jrect);
  static uint16_t loadMem(JDEC *jdec, uint8_t *buf, uint16_t len);
  // jpgScale: 0->no scale, 1->1:2, 2->1:4, 3->1:8
  JRESULT show(int16_t x, int16_t y, const uint8_t *jpg, uint32_t len, int jpgScale=0);
  JRESULT getDim(uint16_t *w, uint16_t *h, const uint8_t *jpg, uint32_t len);

#if defined USE_SDFAT_LIBRARY
  static uint16_t loadSD(JDEC *jdec, uint8_t *buf, uint16_t len);
  JRESULT show(int16_t x, int16_t y, const char *fileName, int jpgScale=0);
  JRESULT getDim(uint16_t *w, uint16_t *h, const char *fileName);
#endif

};

#endif
