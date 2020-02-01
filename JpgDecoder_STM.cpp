// JPEG Decoder for STM32 using TJpgDec
// (c) 2019/20 by Pawel A. Hernik
// Based on Bodmer's code and TJpgDec project:
// http://elm-chan.org/fsw/tjpgd/00index.html

#include "JpgDecoder_STM.h"

bool (*JpgDecoder::renderFun)(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *data);
const uint8_t* JpgDecoder::data = 0;
uint32_t JpgDecoder::dataIdx = 0;
uint32_t JpgDecoder::dataLen = 0;
int16_t JpgDecoder::startX = 0;
int16_t JpgDecoder::startY = 0;

JpgDecoder::JpgDecoder(bool (*fun)(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *data), uint8_t *buf, uint16_t len)
{
  renderFun = fun;
  decodeBuffer = buf;
  decodeBufferLen = len;
}

uint16_t JpgDecoder::loadMem(JDEC* jdec, uint8_t* buf, uint16_t len)
{
  if(dataIdx + len > dataLen) len = dataLen - dataIdx;
  if(buf) memcpy_P(buf, (const uint8_t *)(data + dataIdx), len);
  dataIdx += len;
  return len;
}

uint16_t JpgDecoder::renderLCD(JDEC* jdec, void* bitmap, JRECT* jrect)
{
  int16_t  x = jrect->left + startX;
  int16_t  y = jrect->top  + startY;
  uint16_t w = jrect->right  + 1 - jrect->left;
  uint16_t h = jrect->bottom + 1 - jrect->top;
  return renderFun ? (*renderFun)(x, y, w, h, (uint16_t*)bitmap) : 0;
}

JRESULT JpgDecoder::show(int16_t x, int16_t y, const uint8_t *jpg, uint32_t len, int jpgScale) 
{
  JDEC jdec;
  JRESULT res;
  data    = jpg;
  dataIdx = 0;
  dataLen = len;
  startX  = x;
  startY  = y;

  res = jd_prepare(&jdec, loadMem, decodeBuffer, decodeBufferLen, 0);
  if(res == JDR_OK) res = jd_decomp(&jdec, renderLCD, jpgScale);
  return res;
}

JRESULT JpgDecoder::getDim(uint16_t *w, uint16_t *h, const uint8_t *jpg, uint32_t len) 
{
  JDEC jdec;
  JRESULT res;
  *w = *h = 0;
  dataIdx = 0;
  data  = jpg;
  dataLen  = len;

  res = jd_prepare(&jdec, loadMem, decodeBuffer, decodeBufferLen, 0);
  if(res == JDR_OK) {
    *w = jdec.width;
    *h = jdec.height;
  }

  return res;
}
// --------------------------------------------------
#if defined USE_SDFAT_LIBRARY

SdFile JpgDecoder::jpgSdFile;
  
uint16_t JpgDecoder::loadSD(JDEC *jdec, uint8_t* buf, uint16_t len)
{
  uint32_t bytesLeft = jpgSdFile.available();
  if(bytesLeft < len) len = bytesLeft;
  if(buf) jpgSdFile.read(buf, len); else jpgSdFile.seekCur(len);
  return len;
}

JRESULT JpgDecoder::show(int16_t x, int16_t y, const char *fileName, int jpgScale) 
{
  jpgSdFile.open(fileName, O_CREAT | O_RDONLY);
  JDEC jdec;
  JRESULT res;
  startX = x;
  startY = y;

  res = jd_prepare(&jdec, loadSD, decodeBuffer, decodeBufferLen, 0);
  if(res == JDR_OK) res = jd_decomp(&jdec, renderLCD, jpgScale);
  jpgSdFile.close();
	return res;
}

JRESULT JpgDecoder::getDim(uint16_t *w, uint16_t *h, const char *fileName) 
{
  jpgSdFile.open(fileName, O_CREAT | O_RDONLY);
  JDEC jdec;
  JRESULT res;
  *w = *h = 0;
  res = jd_prepare(&jdec, loadSD, decodeBuffer, decodeBufferLen, 0);

  if(res == JDR_OK) {
    *w = jdec.width;
    *h = jdec.height;
  }

  jpgSdFile.close();
	return res;
}

#endif
