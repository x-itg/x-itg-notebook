//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖÐ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼þ Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : Evk123
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üÐÞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : 0.69´çOLED ½Ó¿ÚÑÝÊ¾Àý³Ì(STM32F103ZEÏµÁÐIIC)
//              ËµÃ÷: 
//              ----------------------------------------------------------------
//              GND   µçÔ´µØ
//              VCC   ½Ó5V»ò3.3vµçÔ´
//              SCL   ½ÓPD6£¨SCL£©
//              SDA   ½ÓPD7£¨SDA£©            
//              ----------------------------------------------------------------
//Copyright(C) ÖÐ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////£

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	
#include "SI4432.h" 
#include "SavePara.h"
const GBX16 Hzk[]={
"Õý", 0x00,0x02,0x02,0xC2,0x02,0x02,0x02,0xFE,
      0x82,0x82,0x82,0xC2,0x83,0x02,0x00,0x00,
      0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x7F,
      0x40,0x40,0x40,0x40,0x40,0x60,0x40,0x00,

"³£", 0x20,0x18,0x08,0x0A,0xEC,0xA8,0xA8,0xAF,
      0xA8,0xA8,0xEC,0x0A,0x08,0x28,0x18,0x00,
      0x00,0x00,0x00,0x3E,0x02,0x02,0x02,0xFF,
      0x02,0x22,0x42,0x3F,0x02,0x00,0x00,0x00,

"´í", 0x80,0x40,0x70,0xCF,0x48,0x48,0x48,0x48,
      0x7F,0x48,0x48,0x7F,0xC8,0x68,0x40,0x00,
      0x00,0x02,0x02,0x7F,0x22,0x12,0x00,0xFF,
      0x49,0x49,0x49,0x49,0xFF,0x01,0x00,0x00,

"Îó", 0x40,0x42,0xC4,0x0C,0x00,0x40,0x5E,0x52,
      0x52,0xD2,0x52,0x52,0x5F,0x42,0x00,0x00,
      0x00,0x00,0x7F,0x20,0x12,0x82,0x42,0x22,
      0x1A,0x07,0x1A,0x22,0x42,0xC3,0x42,0x00,
"ÔË", 0x40,0x42,0x44,0xCC,0x00,0x20,0x22,0x22,
      0xE2,0x22,0x22,0x23,0x32,0x20,0x00,0x00,
      0x00,0x40,0x20,0x1F,0x20,0x48,0x4C,0x4B,
      0x48,0x48,0x4A,0x4C,0x58,0x60,0x20,0x00,

"ÐÐ", 0x00,0x10,0x88,0xC4,0x23,0x40,0x42,0x42,
      0x42,0x42,0x42,0xC2,0x43,0x62,0x40,0x00,
      0x02,0x01,0x00,0xFF,0x00,0x00,0x00,0x00,
      0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,

"×´", 0x00,0x08,0x30,0x00,0xFF,0x20,0x20,0x20,
      0x20,0xFF,0x20,0x22,0x24,0x30,0x20,0x00,
      0x08,0x0C,0x02,0x01,0xFF,0x40,0x20,0x1C,
      0x03,0x00,0x03,0x0C,0x30,0x60,0x20,0x00,

"Ì¬", 0x04,0x04,0x84,0x84,0x44,0x24,0x54,0x8F,
      0x14,0x24,0x44,0x44,0x84,0x86,0x84,0x00,
      0x01,0x21,0x1C,0x00,0x3C,0x40,0x42,0x4C,
      0x40,0x40,0x70,0x04,0x08,0x31,0x00,0x00,
"Ä£", 0x10,0x10,0xD0,0xFF,0x94,0x14,0xE4,0xBF,
      0xA4,0xA4,0xA4,0xBF,0xF4,0x26,0x04,0x00,
      0x04,0x03,0x00,0xFF,0x08,0x89,0x8B,0x4A,
      0x2A,0x1E,0x2A,0x2A,0x4B,0xC8,0x48,0x00,

"¿é", 0x20,0x20,0x20,0xFF,0x20,0x20,0x10,0x10,
      0xFF,0x10,0x10,0x10,0xF8,0x10,0x00,0x00,
      0x08,0x18,0x08,0x0F,0x84,0x46,0x22,0x1A,
      0x07,0x0A,0x12,0x22,0x43,0xC2,0x42,0x00,

"Êý", 0x10,0x92,0x54,0x30,0xFF,0x50,0x94,0x32,
      0xD8,0x17,0x10,0x10,0xF0,0x18,0x10,0x00,
      0x02,0x82,0x4E,0x33,0x22,0x52,0x8E,0x40,
      0x23,0x14,0x08,0x16,0x61,0xC0,0x40,0x00,
"Ð£", 0x10,0x10,0xD0,0xFF,0x90,0x10,0x88,0x48,
      0xA8,0x09,0x0A,0x08,0xA8,0x4C,0xC8,0x00,
      0x04,0x03,0x00,0xFF,0x80,0x81,0x40,0x40,
      0x21,0x16,0x08,0x16,0x61,0xC0,0x40,0x00,

"×¼", 0x00,0x02,0x04,0xEC,0x40,0x20,0xF8,0x4F,
      0x48,0x49,0xFE,0x48,0x68,0x4C,0x08,0x00,
      0x02,0x02,0x7F,0x00,0x00,0x00,0xFF,0x22,
      0x22,0x22,0x3F,0x22,0x23,0x32,0x20,0x00,

"²¨", 0x20,0x42,0x04,0x0C,0x80,0xF8,0x88,0x88,
      0x88,0xFF,0x88,0x88,0xA8,0x18,0x00,0x00,
      0x04,0x04,0xFE,0x41,0x30,0x8F,0x40,0x22,
      0x14,0x08,0x14,0x22,0x41,0xC0,0x40,0x00,

"ÌØ", 0x20,0x1C,0x10,0xFF,0x90,0x50,0x48,0x48,
      0x48,0x7F,0x48,0xC8,0x4C,0x68,0x40,0x00,
      0x02,0x02,0x01,0xFF,0x00,0x02,0x02,0x06,
      0x1A,0x42,0x82,0x7F,0x02,0x03,0x02,0x00,

"ÂÊ", 0x00,0x14,0xA4,0x44,0x04,0x24,0xB5,0x6E,
      0x24,0x94,0x04,0x44,0xA6,0x14,0x00,0x00,
      0x08,0x09,0x08,0x08,0x09,0x09,0x09,0xFF,
      0x09,0x09,0x0B,0x08,0x08,0x0D,0x08,0x00,

"µØ", 0x40,0x40,0xFE,0x40,0x40,0x80,0xFC,0x40,
      0x40,0xFF,0x20,0x20,0xF0,0x20,0x00,0x00,
      0x20,0x60,0x3F,0x10,0x10,0x00,0x3F,0x40,
      0x40,0x5F,0x44,0x48,0x47,0x40,0x70,0x00,

"Ö·", 0x10,0x10,0x10,0xFF,0x10,0x18,0x10,0xF8,
      0x00,0x00,0xFF,0x20,0x20,0x30,0x20,0x00,
      0x20,0x60,0x20,0x3F,0x10,0x50,0x48,0x7F,
      0x40,0x40,0x7F,0x40,0x40,0x60,0x40,0x00,
"¹ý", 0x40,0x42,0x44,0xC8,0x00,0x08,0x08,0x48,
      0x88,0x08,0x08,0xFF,0x08,0x08,0x00,0x00,
      0x00,0x40,0x20,0x1F,0x20,0x40,0x40,0x40,
      0x41,0x48,0x50,0x4F,0x40,0x60,0x20,0x00,

"´¥", 0x20,0xF8,0x27,0xF4,0x2C,0xE4,0x00,0xF0,
      0x10,0x10,0xFF,0x10,0x10,0xF8,0x10,0x00,
      0x80,0x7F,0x09,0x7F,0x89,0xFF,0x00,0x4F,
      0xC4,0x44,0x7F,0x24,0x34,0x27,0xC0,0x00,

"·¢", 0x00,0x10,0x18,0x16,0x10,0x90,0xF0,0x9F,
      0x90,0x90,0x92,0x94,0x10,0x18,0x10,0x00,
      0x40,0x20,0x90,0x88,0x46,0x41,0x23,0x14,
      0x08,0x14,0x22,0x21,0x40,0xC0,0x40,0x00,
"Ç·", 0x80,0x40,0x20,0x18,0x0F,0x08,0x08,0xE8,
      0x08,0x08,0x08,0x48,0x28,0x18,0x00,0x00,
      0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00,
      0x03,0x0C,0x10,0x20,0x40,0xC0,0x40,0x00,

"²¹", 0x10,0x10,0x10,0x11,0xD6,0x30,0x98,0x00,
      0x00,0xFF,0x40,0x80,0x00,0x00,0x00,0x00,
      0x08,0x04,0x02,0x01,0xFF,0x01,0x02,0x00,
      0x00,0xFF,0x00,0x00,0x01,0x03,0x00,0x00,

"³¥", 0x80,0x40,0x20,0xF8,0x47,0x30,0x92,0x94,
      0x90,0x9F,0x90,0xD4,0x92,0x50,0x30,0x00,
      0x00,0x00,0x00,0xFF,0x04,0x04,0x44,0x64,
      0x54,0x4C,0x44,0x54,0x64,0xC6,0x04,0x00,
"Ö±", 0x00,0x04,0x04,0xE4,0xA4,0xA4,0xA4,0xBF,
      0xA4,0xA4,0xA4,0xF4,0x26,0x04,0x00,0x00,
      0x40,0x40,0x40,0x7F,0x4A,0x4A,0x4A,0x4A,
      0x4A,0x4A,0x4A,0x7F,0x40,0x60,0x40,0x00,
"ÊÐ", 0x08,0x08,0xC8,0x48,0x48,0x48,0x49,0xFA,
      0x48,0x48,0x48,0x48,0xE8,0x4C,0x08,0x00,
      0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0xFF,
      0x00,0x00,0x10,0x20,0x1F,0x00,0x00,0x00,

"±¨", 0x10,0x10,0x10,0xFF,0x90,0x50,0x00,0xFE,
      0x82,0x82,0x92,0xA2,0x9F,0x02,0x00,0x00,
      0x02,0x42,0x81,0x7F,0x00,0x00,0x00,0xFF,
      0x22,0x14,0x08,0x16,0x21,0x60,0x20,0x00,

"¾¯", 0x80,0x92,0x8A,0xBF,0xAA,0xBB,0xCA,0xFA,
      0x84,0xA7,0xAA,0x92,0xAE,0xE2,0xA0,0x00,
      0x00,0x00,0x00,0xEA,0xAA,0xAA,0xAA,0xAA,
      0xAA,0xAA,0xAA,0xEA,0x00,0x00,0x00,0x00,
"Àë", 0x04,0x04,0x04,0xF4,0x04,0x8C,0x55,0x26,
      0x54,0x8C,0x04,0xF4,0x04,0x06,0x04,0x00,
      0x00,0xFC,0x04,0x05,0x25,0x35,0x2D,0x27,
      0x25,0x35,0x25,0x45,0x84,0x7E,0x04,0x00,
"ÔÚ", 0x08,0x08,0x08,0x08,0xC8,0x38,0x0F,0x08,
      0x08,0xE8,0x08,0x88,0x08,0x0C,0x08,0x00,
      0x08,0x04,0x02,0xFF,0x00,0x40,0x41,0x41,
      0x41,0x7F,0x41,0x41,0x41,0x60,0x40,0x00,
"Ïß", 0x00,0x20,0x30,0xAC,0x63,0x20,0x30,0x20,
      0x20,0xFF,0x90,0x92,0x94,0xD0,0x80,0x00,
      0x20,0x62,0x23,0x12,0x12,0x12,0x41,0x41,
      0x21,0x17,0x18,0x24,0x42,0x80,0xE0,0x00,
"Ä£", 0x10,0x10,0xD0,0xFF,0x94,0x14,0xE4,0xBF,
      0xA4,0xA4,0xA4,0xBF,0xF4,0x26,0x04,0x00,
      0x04,0x03,0x00,0xFF,0x08,0x89,0x8B,0x4A,
      0x2A,0x1E,0x2A,0x2A,0x4B,0xC8,0x48,0x00,
"¿é", 0x20,0x20,0x20,0xFF,0x20,0x20,0x10,0x10,
      0xFF,0x10,0x10,0x10,0xF8,0x10,0x00,0x00,
      0x08,0x18,0x08,0x0F,0x84,0x46,0x22,0x1A,
      0x07,0x0A,0x12,0x22,0x43,0xC2,0x42,0x00,
"Æô", 0x00,0x00,0x00,0xFC,0x44,0x44,0x44,0x45,
      0x46,0x44,0x44,0x44,0x44,0x7E,0x04,0x00,
      0x40,0x20,0x18,0x07,0xFE,0x42,0x42,0x42,
      0x42,0x42,0x42,0x42,0x42,0xFF,0x02,0x00,
"ÓÃ", 0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0xFE,
      0x22,0x22,0x22,0x22,0xFF,0x02,0x00,0x00,
      0x80,0x60,0x1F,0x02,0x02,0x02,0x02,0x7F,
      0x02,0x02,0x42,0x82,0x7F,0x00,0x00,0x00,
"ÆÁ", 0x00,0x00,0xFE,0x12,0x92,0x92,0xB2,0xD2,
      0x92,0xD2,0xB2,0x92,0xDF,0x82,0x00,0x00,
      0x40,0x20,0x1F,0x04,0x84,0x44,0x3F,0x04,
      0x04,0x04,0xFF,0x04,0x04,0x06,0x04,0x00,
"±Î", 0x04,0x94,0xA4,0x84,0xF4,0xCF,0xA4,0x94,
      0x04,0x8F,0x74,0x44,0x44,0xE6,0x44,0x00,
      0x00,0xFF,0x08,0x04,0x3F,0x04,0x88,0xFF,
      0x81,0x40,0x2F,0x10,0x28,0xC7,0x40,0x00,


"Éè", 0x40,0x40,0x42,0xCC,0x00,0x40,0xA0,0x9F,
      0x81,0x81,0x81,0x9F,0xA0,0x20,0x20,0x00,
      0x00,0x00,0x00,0x7F,0xA0,0x90,0x40,0x43,
      0x2C,0x10,0x28,0x26,0x41,0xC0,0x40,0x00,
"ÖÃ", 0x00,0x10,0x17,0xD5,0x55,0x57,0x55,0x7D,
      0x55,0x57,0x55,0xD5,0x17,0x10,0x00,0x00,
      0x40,0x40,0x40,0x7F,0x55,0x55,0x55,0x55,
      0x55,0x55,0x55,0x7F,0x40,0x60,0x40,0x00,
"³ö", 0x00,0x00,0xFC,0x40,0x40,0x40,0x40,0xFF,
      0x40,0x40,0x40,0x40,0xFC,0x00,0x00,0x00,
      0x00,0xFE,0x40,0x40,0x40,0x40,0x40,0x7F,
      0x40,0x40,0x40,0x40,0x40,0xFE,0x00,0x00,
"¿ª", 0x80,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,
      0x82,0x82,0xFE,0x82,0x83,0xC2,0x80,0x00,
      0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x00,
      0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,
"¹Ø", 0x00,0x10,0x10,0x11,0x12,0x1C,0x10,0xF0,
      0x10,0x18,0x14,0x13,0x1A,0x90,0x00,0x00,
      0x81,0x81,0x41,0x41,0x21,0x11,0x0D,0x03,
      0x0D,0x11,0x21,0x21,0x41,0xC1,0x41,0x00,
"ÎÂ", 0x10,0x22,0x64,0x0C,0x80,0x00,0xFE,0x92,
      0x92,0x92,0x92,0x92,0xFF,0x02,0x00,0x00,
      0x04,0x04,0xFE,0x01,0x40,0x7E,0x42,0x42,
      0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00,
"¶È", 0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0xA5,
      0xA6,0xA4,0xFC,0x24,0x34,0x26,0x04,0x00,
      0x40,0x20,0x9F,0x80,0x42,0x42,0x26,0x2A,
      0x12,0x2A,0x26,0x42,0x40,0xC0,0x40,0x00,
"¡æ", 0x00,0x06,0x09,0x09,0xE6,0xF0,0x18,0x08,
      0x08,0x08,0x18,0x30,0x78,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x07,0x0F,0x18,0x30,
      0x20,0x20,0x20,0x10,0x08,0x00,0x00,0x00,
"ÍË", 0x40,0x42,0x44,0xCC,0x00,0x00,0xFF,0x49,
      0x49,0x49,0x49,0x49,0xFF,0x40,0x00,0x00,
      0x00,0x40,0x20,0x1F,0x20,0x40,0x9F,0x88,
      0x85,0x81,0x82,0x85,0x88,0xD8,0x40,0x00,
"µç", 0x00,0xF8,0x48,0x48,0x48,0x48,0xFF,0x48,
      0x48,0x48,0x48,0xFC,0x08,0x00,0x00,0x00,
      0x00,0x07,0x02,0x02,0x02,0x02,0x3F,0x42,
      0x42,0x42,0x42,0x47,0x40,0x70,0x00,0x00,
"Ñ¹", 0x00,0x00,0xFE,0x02,0x82,0x82,0x82,0x82,
      0xFE,0x82,0x82,0x82,0xC3,0x82,0x00,0x00,
      0x40,0x30,0x0F,0x40,0x40,0x40,0x40,0x40,
      0x7F,0x40,0x42,0x44,0x4C,0x60,0x40,0x00,
"Á÷", 0x10,0x22,0x64,0x0C,0x80,0x44,0x44,0x64,
      0x55,0x4E,0x44,0x54,0x66,0xC4,0x00,0x00,
      0x04,0x04,0xFE,0x01,0x00,0x80,0x40,0x3F,
      0x00,0xFF,0x00,0x3F,0x40,0x40,0x70,0x00,

"²é", 0x84,0x84,0x44,0xE4,0x54,0x4C,0x44,0x7F,
      0x44,0x4C,0x54,0xE4,0x44,0xC6,0x44,0x00,
      0x40,0x40,0x40,0x5F,0x52,0x52,0x52,0x52,
      0x52,0x52,0x52,0x5F,0x40,0x60,0x40,0x00,

"¿´", 0x20,0x2A,0x2A,0xAA,0xEA,0xBA,0xAE,0xAA,
      0xAA,0xA9,0xA9,0xED,0xA8,0x20,0x20,0x00,
      0x04,0x02,0x01,0xFF,0x4A,0x4A,0x4A,0x4A,
      0x4A,0x4A,0x4A,0xFF,0x00,0x00,0x00,0x00,

"½»", 0x08,0x08,0x88,0x48,0xB8,0x08,0x09,0x0E,
      0x08,0x08,0x98,0x28,0x48,0xCC,0x08,0x00,
      0x80,0x81,0x40,0x40,0x20,0x11,0x0A,0x04,
      0x0A,0x11,0x20,0x20,0x40,0xC0,0x40,0x00
};

void SDA_Out(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//Êä³ö
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
//ÉèÖÃÊý¾ÝÏßÎªÊäÈë
void SDA_In(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;//ÊäÈë
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


//--------------------------------------------------------
//Æô¶¯×ÜÏß
//--------------------------------------------------------

void IIC_Start()
{
  SDA_Out();
	SDA_H;
	SCL_H;
	delay_us(2);
	SDA_L;
	delay_us(2);
	SCL_L;
	delay_us(2);
}
//--------------------------------------------------------
//ÊÍ·Å×ÜÏß
//--------------------------------------------------------

void IIC_Stop()
{
 IIC_Start();
	SCL_H;
	SDA_L;
	delay_us(2);
	SDA_H;
	delay_us(2);
}
//------------------------------------------------------
//¶ÁÓ¦´ð
//------------------------------------------------------

unsigned char Read_Ask(void)
{
	unsigned char ReAsk;
	SDA_In();
	SCL_H;
	delay_us(2);
	ReAsk=(unsigned char)SDA_READ;
	SCL_L;
	delay_us(2);
	return ReAsk;
}
//-----------------------------------------------------
//Ð´ÈëÒ»¸ö×Ö½Ú
//-----------------------------------------------------
void Write_OneByte(unsigned char Dat)
{
  unsigned char i;
  SDA_Out();
  for(i=0;i<8;i++)
  {
    if(Dat&0x80){SDA_H;}
    else{SDA_L;}
    Dat<<=1;
    SCL_H;
    delay_us(2);
    SCL_L;
    delay_us(2);
  }
  Read_Ask();
}	

//------------------------------------------------------
//Ð´ÃüÁî
//------------------------------------------------------
void WriteCmd(unsigned char IIC_Cmd)
{
  IIC_Start();
  Write_OneByte(0x78);
	Write_OneByte(0x00);
	Write_OneByte(IIC_Cmd);
	IIC_Stop();

}
//------------------------------------------------------
//Ð´Êý¾Ý
//------------------------------------------------------
void WriteDat(unsigned char IIC_Dat)
{
  IIC_Start();
  Write_OneByte(0x78);
	Write_OneByte(0x40);
	Write_OneByte(IIC_Dat);
	IIC_Stop();
}
//-----------------------------------------------------
//OLED³õÊ¼»¯
//-----------------------------------------------------
void OLED_Init(void)
{
  SDA_Out();
  
  delay_ms(800);
	WriteCmd(0xae);
	WriteCmd(0x00);
	WriteCmd(0x10);
	WriteCmd(0x40);
	WriteCmd(0x81);
	WriteCmd(0xcf);
	WriteCmd(0xa1);
	WriteCmd(0xc8);
	WriteCmd(0xa6);
	WriteCmd(0xa8);
	WriteCmd(0x3f);
	WriteCmd(0xd3);
	WriteCmd(0x00);
	WriteCmd(0xd5);
	WriteCmd(0x80);
	WriteCmd(0xd9);
	WriteCmd(0xf1);
	WriteCmd(0xda);
	WriteCmd(0x12);
	WriteCmd(0xdb);
	WriteCmd(0x40);
	WriteCmd(0x20);
	WriteCmd(0x02);
	WriteCmd(0x8d);
	WriteCmd(0x14);
	WriteCmd(0xa4);
	WriteCmd(0xa6);
	WriteCmd(0xaf);
}


//-----------------------------------------------------------------------
//OLEDÈ«ÆÁÌî³ä
//----------------------------------------------------------------------
void OLED_Fill(unsigned char fill_Data)
{
 unsigned char m,n;
	for(m=0;m<8;m++)
	{
	 WriteCmd(0xb0+m);
	 WriteCmd(0x00);
	 WriteCmd(0x10);
	 for(n=0;n<128;n++)
		{
		WriteDat(fill_Data);
		}
	}
}
//--------------------------------------------------------------
void OLED_CLS(void)
{
 OLED_Fill(0x00);

}
//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : ½«OLED´ÓÐÝÃßÖÐ»½ÐÑ
//--------------------------------------------------------------

void OLED_ON(void)
{
 WriteCmd(0x8d);
 WriteCmd(0x10);
 WriteCmd(0xae);
}


//-------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : ÈÃOLEDÐÝÃß 
//------------------------------------------------------------
void OLED_OFF(void)
{
  WriteCmd(0x8d);
  WriteCmd(0x10);
  WriteCmd(0xae);
}


//----------------------------------------------------------------------
//ÉèÖÃ×ø±ê
//----------------------------------------------------------------------
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)); 
}   

//ÏÔÊ¾ºº×Ö
//È¡Ä£·½Ê½ ×ÝÏò8µãÉÏ¸ßÎ»ÖÃ
//OLED_ShowCHinese (i*8+8*ucX,ucY,(unsigned char *)&Hzk[0].Msk[0],revyn);
void OLED_ShowCHinese(unsigned char  x,unsigned char  y,unsigned char *chn,unsigned char revshow)
{      			    
  unsigned char t,adder=0;
  if(revshow==0)
  {
  OLED_Set_Pos(x,y);	
  for(t=0;t<16;t++)
  {
    WriteDat(*chn++);
      adder+=1;
      
  }	
  OLED_Set_Pos(x,y+1);	
  for(t=0;t<16;t++)
  {	
    WriteDat(*chn++);
    adder+=1;
 
  }
  }else
  {
    OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
    {
      WriteDat(~(*chn++));
      adder+=1;
 
    }	
  OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
    {	
      WriteDat(~(*chn++));
      adder+=1;
 
    }
  }				
}






//-----------------------------------------------------------------------------





static  unsigned char str[16] = {
'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static char cabs(char val)
{

  return (val > 0 ? val:(val*-1));

}



void Char2Str(signed char Data, unsigned char Len, unsigned char DecLen, unsigned char *pStr)
{
	unsigned char i;
	unsigned char minus;
	unsigned char uData;

	//validity check
	*pStr = '\0';
	if(Len <= DecLen) return;
//	if(DecLen == 0 && Len <= 1) return;
	if(DecLen && Len < (DecLen + 2)) return;

	uData = cabs(Data);
	if(Data < 0)  minus = '-';
    else  minus = ' ';

	pStr += Len;
	*pStr-- = '\0';
	i = 0;
	//decimal portion
	while(i < DecLen)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
		
	//decimal point
	if(DecLen)
	{
		*pStr-- = '.';
		i++;
	}
	//integer before point.
	*pStr-- = uData % 10 + '0';
	uData /= 10;
	i++;
	//rest integer
	while(i < Len -1 && uData)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
 	//sign
	*pStr-- = minus;
	i++;
	//rest blank
	while(i < Len)
	{
		*pStr-- = ' ';
		i++;
	}
}

void uChar2Hex(unsigned char uData, unsigned char Len, unsigned char DecLen, unsigned char *pStr)
{
	unsigned char i;

	//validity check
	*pStr = '\0';
	if(Len <= DecLen) return;
//	if(DecLen == 0 && Len <= 1) return;
	if(DecLen && Len < (DecLen + 2)) return;


	pStr += Len;	// move to head postion
	*pStr-- = '\0';
	i = 0;
	//decimal portion
	while(i < DecLen)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
		
	//decimal point
	if(DecLen)
	{
		*pStr-- = '.';
		i++;
	}
	//integer before point.
	*pStr-- = str[uData % 16];
	uData /= 16;
	i++;
	//rest integer
	while(i < Len && uData)
	{
		*pStr-- = str[uData % 16];
		uData /= 16;
		i++;
	}
	//rest blank
	while(i < Len)
	{
		*pStr-- = '0';
		i++;
	}
}

void uChar2Str(unsigned char uData, unsigned char Len, unsigned char DecLen, unsigned char *pStr)
{
	unsigned char i;

	//validity check
	*pStr = '\0';
	if(Len <= DecLen) return;
//	if(DecLen == 0 && Len <= 1) return;
	if(DecLen && Len < (DecLen + 2)) return;


	pStr += Len;
	*pStr-- = '\0';
	i = 0;
	//decimal portion
	while(i < DecLen)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
		
	//decimal point
	if(DecLen)
	{
		*pStr-- = '.';
		i++;
	}
	//integer before point.
	*pStr-- = uData % 10 + '0';
	uData /= 10;
	i++;
	//rest integer
	while(i < Len && uData)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
	//rest blank
	while(i < Len)
	{
		*pStr-- = ' ';
		i++;
	}
}



//Len: total length of the string. include decimal portion, decimal point, integer portion and sign.
//DecLen exclude decimal point.
void Int2Str(signed short Data, unsigned char Len, unsigned char DecLen, unsigned char *pStr)
{
	unsigned char i;
	unsigned char minus;
	unsigned int uData;

	//validity check
	*pStr = '\0';
	if(Len <= DecLen) return;
//	if(DecLen == 0 && Len <= 1) return;
	if(DecLen && Len < (DecLen + 2)) return;

	uData = abs(Data);
	if(Data < 0)  minus = '-';
    else  minus = ' ';

	pStr += Len;
	*pStr-- = '\0';
	i = 0;
	//decimal portion
	while(i < DecLen)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
		
	//decimal point
	if(DecLen)
	{
		*pStr-- = '.';
		i++;
	}
	//integer before point.
	*pStr-- = uData % 10 + '0';
	uData /= 10;
	i++;
	//rest integer
	while(i < Len -1 && uData)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
 	//sign
	*pStr-- = minus;
	i++;
	//rest blank
	while(i < Len)
	{
		*pStr-- = ' ';
		i++;
	}
}



void uInt2Str(unsigned short uData, unsigned char Len, unsigned char DecLen, unsigned char *pStr)
{
	unsigned char i;

	//validity check
	*pStr = '\0';
	if(Len <= DecLen) return;
//	if(DecLen == 0 && Len <= 1) return;
	if(DecLen && Len < (DecLen + 2)) return;


	pStr += Len;
	*pStr-- = '\0';
	i = 0;
	//decimal portion
	while(i < DecLen)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
		
	//decimal point
	if(DecLen)
	{
		*pStr-- = '.';
		i++;
	}
	//integer before point.
	*pStr-- = uData % 10 + '0';
	uData /= 10;
	i++;
	//rest integer
	while(i < Len && uData)
	{
		*pStr-- = uData % 10 + '0';
		uData /= 10;
		i++;
	}
	//rest blank
	while(i < Len)
	{
		*pStr-- = ' ';
		i++;
	}
}



//8x16  ¸ß16 
void OLED_ShowCharNomAndRev(unsigned char x,unsigned char y,unsigned char chr,unsigned char rev)
{      	
  unsigned char c=0,i=0;	
  c=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ			
  if(x>128-1){x=0;y=y+2;}//×î¶à128ÁÐ
  if(rev ==0)
  {
    OLED_Set_Pos(x,y);	
    for(i=0;i<8;i++)
    WriteDat(F8X16[c*16+i]);
    OLED_Set_Pos(x,y+1);
    for(i=0;i<8;i++)
      WriteDat(F8X16[c*16+i+8]);
  }
  else 
  {	
     OLED_Set_Pos(x,y);	
     for(i=0;i<8;i++)
     WriteDat(~F8X16[c*16+i]);
     OLED_Set_Pos(x,y+1);
     for(i=0;i<8;i++)
       WriteDat(~F8X16[c*16+i+8]);
    }
}
//-----------------------------------------------------------------------------
//²Ëµ¥µ÷ÓÃÕâ¸öº¯Êý
int PrintHZ(unsigned short  ucY, unsigned short ucX, const unsigned char *ptr, unsigned short ucRevX, unsigned char ucDispMode, unsigned short ucRevLen, unsigned char ucCodeGen)
{
  unsigned short Len=0;
  unsigned short i=0;
  unsigned short j=0;//yÓÃÀ´È·¶¨µÚ¼¸¸öºº×Ö
  unsigned short k=sizeof(Hzk)/sizeof(Hzk[0]);//Ò»¹²ÓÐ¼¸¸öºº×Ö
  unsigned char c1,c2;
  unsigned char revyn;
  unsigned char revynchar;
 
  while (ptr[Len]!=0) //Ì½²â×Ö´®³¤¶È
  {
     Len++;
  }	
  while(i<Len)
  {
    c1 = ptr[i];
    c2 = ptr[i+1];
    
    
    if(c1>128)//ÊÇºº×Ö
    {
      for(j=0;j <k;j++)  	//ÕÒµ½×ÖÄ£ÐÍ
      {
	if(c1 == Hzk[j].Index[0] && c2 == Hzk[j].Index[1])
	{
		break;
	}
      }
      
      if(ucDispMode==1&&(i+ucX)>=ucRevX&&(i+ucX)<ucRevX+2*ucRevLen)
    {
        revyn=1;
    }else
    {
       revyn=0;
    }
      
      if(j==k&&c1!=Hzk[j].Index[0] || c2 != Hzk[j].Index[1])
      {
         
          if((i+ucX)<32)
          {
            OLED_ShowCHinese (i*8+8*ucX,2*ucY,(unsigned char *)&Hzk[0].Msk[0],revyn); //ÊµÔÚÕÒ²»µ½¾Í´òÓ¡µÚÒ»¸öºº×Ö
          }
         
      }else
      {
         
          if((i+ucX)<32)
          {
            OLED_ShowCHinese (i*8+8*ucX,2*ucY,(unsigned char *)&Hzk[j].Msk[0],revyn);
          }
         
      }
      
      i++;//ºº×ÖµÄ»°¶àÉ¨ÃèÒ»´Î
      
    }else
    {
      if(ucDispMode==1&&(i+ucX)>=ucRevX&&(i+ucX)<ucRevX+ucRevLen)
      {
        revynchar=1;
      }else
      {
       revynchar=0;
      }
      
      if((i+ucX)<32)
      {
        OLED_ShowCharNomAndRev(i*8+8*ucX,2*ucY,ptr[i],revynchar);
      }
    }
    
    i++;//ÖÐÓ¢ÎÄ¶¼ÒªÉ¨Ãè
  }
  return 1;
}














////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
const REC_FRAME  PageStart[];
const REC_FRAME  PageHome[];
const REC_FRAME  PageView[];
const REC_FRAME  PageSet[];
const unsigned char   *OnlineCode[] = {
  "ÀëÏß",
  "ÔÚÏß"
  };
const unsigned char   *OffOnCode[] = {
  "¹Ø",
  "¿ª"
  };
 
const unsigned char   *OFCode[] = {
  "ÆÁ±Î",                                      
  "ÆôÓÃ"
};

const unsigned char   *SysCode[] = {
  "Õý³£",                                      
  "Ö±Á÷Ç·Ñ¹",
  "Ö±Á÷¹ýÑ¹"
};

const unsigned char   *BaudRateCode[] = {
  "4800 ",
  "9600 ",
  "14400",
  "19200",
  "24000",
  "28800",
  "33600",
  "38400",
  "115200"
  };
 

//ÏÂÃæÊÇ²Ëµ¥²¿·ÖÁË
unsigned char SpaceLine[] = "                ";//string length must as same as M_LCDWIDTH
unsigned char  NullCode[][2] = {""};
unsigned char NullEvent(void){return TRUE;}
unsigned char StartMenuCnt;
static const REC_FRAME  *pCurRec, *pScrTopRec, *pTopRec, *pTmpCurRec;// *pEndRec;
MENU_STA MenuSta = MENU_DISP;
static char CurLine = 0, DigitNum = 0;
static unsigned char ucFlushCount = 0;
static unsigned char StartMenu = 1;
unsigned int   Menu1Ms=0;
unsigned char  Menu1Sec=0;
KEYBUF_FARME keyBuf;
 



//5sÍ£ÁôÒ³Ãæ	
const REC_FRAME  PageStart[] = {
	 //*pTitle             ucTitleX  ucTitleRevX   ucTitleRevLen   *pVar                       ucVarX    ucVarLen  ucVarDecLen   VarType    siVarMax               siVarMin             **pEnumStr                                                                                     *pEvent                    RecRange          LineRange         RecType           *pLinkEnter           *pLinkEsc        
        { "################",       0,        0,           0,             NULL,                        0,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                                NullEvent,                       REC_TOP,          LINE_TOPEND,      RECTP_LINK,       PageHome,             PageHome,        },
	{ " System Booting ",       0,        0,           0,             NULL,                        0,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                                NullEvent,                       REC_MIDDLE,       LINE_TOPEND,      RECTP_LINK,       PageHome,             PageHome,        },
 	{ " Please Wait    ",       0,        0,           0,             NULL,                        0,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                                NullEvent,                       REC_MIDDLE,       LINE_TOPEND,      RECTP_LINK,       PageHome,             PageHome,        },
 	{ "################",       0,        0,           0,             NULL,                        0,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                                NullEvent,                       REC_MIDDLE,       LINE_TOPEND,      RECTP_LINK,       PageHome,             PageHome,        },
	{ "################",       0,        0,           0,             NULL,                        0,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                                NullEvent,                       REC_END,          LINE_TOPEND,      RECTP_LINK,       PageHome,             PageHome,        }
        };

//Ö÷Ò³    ¼ÇÂ¼±í
const REC_FRAME  PageHome[] = {
	//*pTitle               ucTitleX     ucTitleRevX  ucTitleRevLen  *pVar                      ucVarX    ucVarLen  ucVarDecLen   VarType     siVarMax              siVarMin             **pEnumStr                                                                                 *pEvent                      RecRange          LineRange         RecType           *pLinkEnter           *pLinkEsc       
        { "ÔÚÏßÄ£¿éÊý:     ",       0,        0,           10,           &OnLineNum,                   12,       2,        0,           V_UCHAR,        15,                  0,                   NULL,                                                                              NullEvent,                        REC_TOP,          LINE_TOPEND,      RECTP_NOCUR,       NULL,                 PageHome        },
        { "Ö±Á÷µçÑ¹:      V",       0,        0,           5,            &MainPara.DcV,                9,        5,        1,           V_USHORT,      1000,                 0,                   NULL,                                                                                NullEvent,                    REC_MIDDLE,         LINE_TOPEND,      RECTP_NOCUR,       NULL,                 PageHome        },
        { "×´Ì¬:           ",       0,        0,           5,            &MainPara.SysSta,             8,        5,        0,           V_ENUM,       1,                     0,       (unsigned char **)SysCode,                                                                   NullEvent,                     REC_MIDDLE,          LINE_TOPEND,      RECTP_NOCUR,       NULL,                 PageHome         },
        { "[Ä£¿é]  ",               0,        1,           4,             NULL,                        5,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                               NullEvent,                        REC_MIDDLE,        LINE_TOP,         RECTP_LINK,       PageView,             PageHome        },
	{ "  [ÉèÖÃ]",               8,        11,          4,              NULL,                       5,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                              NullEvent,                        REC_MIDDLE,         LINE_END,         RECTP_LINK,       PageSet,              PageHome        },
	{ "                ",       0,        0,           0,             NULL,                        0,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                                NullEvent,                        REC_END,          LINE_TOPEND,      RECTP_LINK,       PageHome,              PageHome       },
	};

//ÏêÇé

extern unsigned  char iMenuNum;                 //ÒªÏÔÊ¾µÄÊý¾ÝºÅ¸úµØÖ·¶ÔÓ¦

 
const REC_FRAME  PageView[] = {
	 //*pTitle             ucTitleX     ucTitleRevX    ucTitleRevLen   *pVar                   ucVarX    ucVarLen  ucVarDecLen   VarType      siVarMax             siVarMin           **pEnumStr                                                                                      *pEvent                    RecRange              LineRange     RecType         *pLinkEnter            *pLinkEsc                     
        { "²é¿´:  [   ]    ",       0,        0,           4,             &iMenuNum,              8,        2,        0,           V_UCHAR,       0x0f,                      0,                   NULL,                                                                                NullEvent,                       REC_TOP,          LINE_TOP,         RECTP_EDIT,       NULL,                 PageHome        },
        { NullCode[0],              0,        0,           1,             &iMenuNumOnlineFlag,    12,       4,        0,           V_ENUM,        2,                       0,           (unsigned char **)OnlineCode,                                                            NullEvent,                       REC_MIDDLE,        LINE_END,        RECTP_NOCUR,       NULL,                 PageHome        },
        { "µçÑ¹A:         V",       0,        0,           5,             &iMenu.V1,              9,        5,        0,           V_USHORT,      1000,                    0,                   NULL,                                                                                NullEvent,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_NOCUR,       NULL,                 PageHome        },
       // { "Ð£×¼:           ",       0,        0,           5,             &MenuCoefficient[0],    9,        5,        0,           V_USHORT,      VcoeMax,                VcoeMin,              NULL,                                                                                FlashUpdateCoe,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_EDIT,       NULL,                 PageHome        },
        { "µçÑ¹B:         V",       0,        0,           5,             &iMenu.V2,              9,        5,        0,           V_USHORT,      1000,                    0,                   NULL,                                                                                NullEvent,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_NOCUR,       NULL,                 PageHome        },
       // { "Ð£×¼:           ",       0,        0,           5,             &MenuCoefficient[1],    9,        5,        0,           V_USHORT,      VcoeMax,                VcoeMin,              NULL,                                                                                FlashUpdateCoe,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_EDIT,       NULL,                 PageHome        },
        { "µçÑ¹C:         V",       0,        0,           5,             &iMenu.V3,              9,        5,        0,           V_USHORT,      1000,                    0,                   NULL,                                                                                NullEvent,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_NOCUR,       NULL,                 PageHome        },
       // { "Ð£×¼:           ",       0,        0,           5,             &MenuCoefficient[2],    9,        5,        0,           V_USHORT,      VcoeMax,                VcoeMin,              NULL,                                                                                FlashUpdateCoe,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_EDIT,       NULL,                 PageHome        },
        { "µçÁ÷A:         A",       0,        0,           5,             &iMenu.I1,              9,        5,        0,           V_USHORT,      1000,                    0,                   NULL,                                                                                NullEvent,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_NOCUR,       NULL,                 PageHome        },
       // { "Ð£×¼:           ",       0,        0,           5,             &MenuCoefficient[3],    9,        5,        0,           V_USHORT,      IcoeMax,                IcoeMin,              NULL,                                                                                FlashUpdateCoe,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_EDIT,       NULL,                 PageHome        },
        { "µçÁ÷B:         A",       0,        0,           5,             &iMenu.I2,              9,        5,        0,           V_USHORT,      1000,                    0,                   NULL,                                                                                NullEvent,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_NOCUR,       NULL,                 PageHome        },
       // { "Ð£×¼:           ",       0,        0,           5,             &MenuCoefficient[4],    9,        5,        0,           V_USHORT,      IcoeMax,                IcoeMin,              NULL,                                                                                FlashUpdateCoe,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_EDIT,       NULL,                 PageHome        }, 
        { "µçÁ÷C:         A",       0,        0,           5,             &iMenu.I3,              9,        5,        0,           V_USHORT,      1000,                    0,                   NULL,                                                                                NullEvent,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_NOCUR,       NULL,                 PageHome        },
       // { "Ð£×¼            ",       0,        0,           5,             &MenuCoefficient[5],    9,        5,        0,           V_USHORT,      IcoeMax,                IcoeMin,              NULL,                                                                                FlashUpdateCoe,                    REC_MIDDLE,          LINE_TOPEND,        RECTP_EDIT,       NULL,                 PageHome        },        
        { "-----[ÍË³ö]---- ",       0,        6,           4,             NULL,                   9,        0,        0,           V_NOP,        0,                    0,                   NULL,                                                                               NullEvent,                       REC_MIDDLE,       LINE_TOPEND,      RECTP_LINK,       PageHome,             PageHome        },
	{ "                 ",      0,        0,           0,             NULL,                       0,        0,        0,           V_NOP,        0,                    0,                   NULL,                                                                                NullEvent,                      REC_END,          LINE_TOPEND,      RECTP_NOCUR,      NULL,                 PageHome        }

        };

//Í¨ÓÃÉèÖÃ
const REC_FRAME  PageSet[] = {
	//*pTitle                ucTitleX     ucTitleRevX     ucTitleRevLen   *pVar                  ucVarX    ucVarLen  ucVarDecLen  VarType       siVarMax              siVarMin            **pEnumStr                                                                                    *pEvent                 RecRange          LineRange        RecType          *pLinkEnter            *pLinkEsc        
        { "ÊÐµçA±¨¾¯:      ",        0,        0,           5,            &SavePara.Awarm,            10,      5,        0,           V_ENUM,       WarnOnOffMax,   WarnOnOffMin,       (unsigned char **)OFCode,                                                                   FlashUpdate,                     REC_TOP,         LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "ÊÐµçB±¨¾¯:      ",        0,        0,           5,            &SavePara.Bwarm,            10,      5,        0,           V_ENUM,       WarnOnOffMax,   WarnOnOffMin,       (unsigned char **)OFCode,                                                                   FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "ÊÐµçC±¨¾¯:      ",        0,        0,           5,            &SavePara.Cwarm,            10,      5,        0,           V_ENUM,       WarnOnOffMax,   WarnOnOffMin,       (unsigned char **)OFCode,                                                                   FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "Ö±Á÷±¨¾¯:       ",        0,        0,           5,            &SavePara.DcWarm,           10,      5,        0,           V_ENUM,       WarnOnOffMax,   WarnOnOffMin,       (unsigned char **)OFCode,                                                                   FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "Ç·Ñ¹²¹³¥:       ",        0,        0,           5,            &SavePara.DacOut,           10,      5,        0,           V_ENUM,       WarnOnOffMax,   WarnOnOffMin,       (unsigned char **)OFCode,                                                                   FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "²¹³¥µçÑ¹:      V",        0,        0,           5,            &SavePara.DacVal,           9,       5,        1,           V_UCHAR,      DacValMax,       DacValMin,                  NULL,                                                                              FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "Ö±Á÷¹ýÑ¹:      V",        0,        0,           5,            &SavePara.DcHig,            9,       5,        1,           V_USHORT,     DcHigMax,         DcHigMin,                  NULL,                                                                              FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "Ö±Á÷Ç·Ñ¹:      V",        0,        0,           5,            &SavePara.DcLow,            9,       5,        1,           V_USHORT,     DcLowMax,         DcLowMin,                  NULL,                                                                              FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "Ö±Á÷´¥·¢:      V",        0,        0,           5,            &SavePara.DcRig,            9,       5,        1,           V_USHORT,     DcRigMax,         DcRigMin,                  NULL,                                                                              FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "½»Á÷¹ýÑ¹:      V",        0,        0,           5,            &SavePara.AcHig,            9,       5,        0,           V_USHORT,     AcHigMax,         AcHigMin,                  NULL,                                                                              FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "½»Á÷Ç·Ñ¹:      V",        0,        0,           5,            &SavePara.AcLow,            9,       5,        0,           V_UCHAR,      AcLowMin,         AcLowMin,                  NULL,                                                                              FlashUpdate,                     REC_MIDDLE,      LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "²¨ÌØÂÊ:         ",        0,        0,           5,            &SavePara.Bdrate,           10,      5,        0,           V_ENUM,       BdrateMax,        BdrateMin,       (unsigned char **)BaudRateCode,                                                             FlashUpdateBrate,                    REC_MIDDLE,       LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
	{ "µØÖ·:           ",        0,        0,           4,            &SavePara.Addr,             10,      3,        0,           V_UCHAR,      AddrMax,         AddrMin,                   NULL,                                                                              FlashUpdate,                    REC_MIDDLE,       LINE_TOPEND,     RECTP_EDIT,       NULL,                 PageHome         },
        { "-----[ÍË³ö]---- ",        0,        6,           2,             NULL,                      0,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                              NullEvent,                     REC_MIDDLE,       LINE_TOPEND,     RECTP_LINK,      PageHome,              PageHome         },
        { "                ",        0,        0,           0,             NULL,                      0,       0,        0,           V_NOP,        0,                    0,                   NULL,                                                                              NullEvent,                    REC_END,          LINE_TOPEND,     RECTP_NOCUR,      NULL,                 PageHome         }

	};


void getKey(KEYTYPE_FRAME *pwhichKey, KEYSTA_FRAME *pKeySta, unsigned int * pkeyNum)
{
	 static KEYSTA_FRAME StatusN = KEY_STA_NULL;
	static KEYSTA_FRAME StatusLast = KEY_STA_NULL;

	*pkeyNum = keyBuf.keyNum;
	keyBuf.keyNum = 0;

	StatusN = keyBuf.ekeySta;
	if(StatusN == KEY_STA_RELEASE && StatusLast == KEY_STA_RELEASE)
		*pKeySta = KEY_STA_NULL;
	else
		*pKeySta = StatusN;
	StatusLast = StatusN;

	switch(keyBuf.keyScanCode)//////////////////////////////////////////
	{
          case 1:
            (*pwhichKey) = KEY_ESC;break;
          case 2:
            (*pwhichKey) = KEY_DOWN;break;
          case 3:
            (*pwhichKey) = KEY_ENTER;break;
          case 4:
            (*pwhichKey) = KEY_UP;break;
	  default:
            (*pwhichKey) = KEY_NULL;break;

        }
	
}

void keyScan(void)
{
  __IO unsigned short keyCodeTmp=0;
  static unsigned int keyPushCnt = 0;
  static unsigned int keyNumCnt;
  

  if(KEYxLEFT)
  {
    keyCodeTmp=1;
    
  }else if(KEYxDOWN)
  {
    keyCodeTmp=2;
    
  }else if(KEYxRIGHT)
  {
    keyCodeTmp=3;
    
  }else if(KEYxUP)
  {
    keyCodeTmp=4;
    
  }else
  {
    keyCodeTmp=0;
  }

  
  if(keyCodeTmp != 0)//ÊÇ·ñÓÐ°´¼ü
  {	
    keyPushCnt++;		        // Á¬Ðø°´¼ü¼ÆÊý	
    
    keyNumCnt++;			// Çø¼äÄÚ°´¼ü¼ÆÊý
    
    keyBuf.keyScanCode = keyCodeTmp;//////////////////////////////
    
    if(keyPushCnt == 20)
    {
      keyBuf.ekeySta = KEY_STA_PUSH;
      keyBuf.keyNum = 1;
      return;
    }
    if(keyPushCnt > 15000)
    {
      keyBuf.keyNum++;
      return;
    }
    if(keyPushCnt > 5000)
    {
      if(keyNumCnt > 5)
      {
        keyNumCnt = 0;
        keyBuf.keyNum++;
      }
      return;
    }
    if(keyPushCnt > 500)
    {
      if(keyNumCnt > 100)
      {
        keyBuf.keyNum++;
        keyNumCnt = 0;
      }
      return;
    }
  }else//ËÉµôÁË Ã»°´¼üÁË
   {
    keyPushCnt = 0;
    keyNumCnt = 0;
    keyBuf.keyNum = 0;
    if(keyBuf.ekeySta == KEY_STA_PUSH)
    {
      keyBuf.ekeySta = KEY_STA_RELEASE;//Èç¹û°´ÏÂ¾Í±äËÉ
    }
  }
}

void MenuInit(void)
{
 
  
  Menu1Sec = 0;
  pCurRec = PageStart;
  pScrTopRec = PageStart+1;
  pTopRec = PageStart;
  pTmpCurRec = NULL;
  MenuSta = MENU_DISP;
  CurLine = 0;
  DigitNum = 0;
  
  
 

}

//display screen title
void PageTitleDisp(const REC_FRAME *pTopTitleRec, const REC_FRAME *pCurTitleRec, const MENU_STA MenuTitleSta)
{
	const REC_FRAME *pTmpRec, *pRecTmp;

	unsigned char ucLine, ucCurInTop;
	
	//first line is fixed
	ucCurInTop = 0;

	pTmpRec = pTopTitleRec;//µÚÒ»Ìõ ¼ÇÂ¼ µÄÊ×µØÖ·  pTmpRec++ºó¼´Ö¸ÏòÏÂÒ»Ìõ¼ÇÂ¼
        
         
	while(pTmpRec->RecRange != REC_TOP)//µÚÒ»Ìõ¼ÇÂ¼µÄÊôÐÔ ¿Ï¶¨ÎªREC_TOP   
    {
         pTmpRec--;
    }
        
	ucLine = 0;
        
 
	while(ucLine == 0)//ÐÐ    É¨Ãæ
	{
		if(pTmpRec == pCurTitleRec && MenuTitleSta == MENU_DISP)//Ñ¡ÖÐ·´ÏÔ
		{
                        PrintHZ(0, pTmpRec->ucTitleX, pTmpRec->pTitle, pTmpRec->ucTitleRevX, REVERSE, pTmpRec->ucTitleRevLen, CGROM);
		}
		else
		{
                        PrintHZ(0, pTmpRec->ucTitleX, pTmpRec->pTitle, pTmpRec->ucTitleRevX, NORMAL, pTmpRec->ucTitleRevLen, CGROM);

		}
		if(pTmpRec == pCurTitleRec)  
                {
                  ucCurInTop = 1;
                 }
		if(pTmpRec->LineRange == LINE_TOPEND || pTmpRec->LineRange == LINE_END)//ÕâÐÐµ½Í·ÁË 
                {
                  ucLine++;//Ìø³öÕâ¸öwhile
                }
		
                pTmpRec++;//ÏÂÒ»¸ö¼ÇÂ¼
	}

	//line 1 to 6 can scroll, if line has none record to fill, fill in space
	if(ucCurInTop == 0)  
        {
          pTmpRec = pTopTitleRec;  //current record isn't in first line
	}
        
        ucLine = M_LCDTOPSCRLINE;
        
	while(ucLine <= M_LCDENDSCRLINE)
	{
		//find end record
		if(pTmpRec->LineRange == LINE_TOP || pTmpRec->LineRange == LINE_TOPEND)
		{
			pRecTmp = pTmpRec;

                        while(pRecTmp->LineRange == LINE_MIDDLE || pRecTmp->LineRange == LINE_TOP)  
                        {   
                          pRecTmp++;
                        }
			if(pRecTmp->RecRange == REC_END)  
                        {
                           break;//Ö±½ÓÌø³ö Õâ¸öÑ­»·////////////////////////////////////////
                        }
		}
		
		if(pTmpRec == pCurTitleRec && MenuTitleSta == MENU_DISP)
		{
                        PrintHZ(ucLine, pTmpRec->ucTitleX, pTmpRec->pTitle, pTmpRec->ucTitleRevX, REVERSE, pTmpRec->ucTitleRevLen, CGROM);

		}
		else
		{
                       PrintHZ(ucLine, pTmpRec->ucTitleX, pTmpRec->pTitle, pTmpRec->ucTitleRevX, NORMAL, pTmpRec->ucTitleRevLen, CGROM);
		}
		if(pTmpRec->LineRange == LINE_TOPEND || pTmpRec->LineRange == LINE_END)  
                {
                  ucLine += 1;
                }
		if(ucLine > M_LCDENDSCRLINE) 
                {
                  break;
                }
		pTmpRec++;
	}
        
	while(ucLine <= M_LCDENDSCRLINE)
	{
          //Print(pTmpRec->ucTitleX, ucLine, SpaceLine, 0, NORMAL, 0, CGROM);
          PrintHZ(ucLine, pTmpRec->ucTitleX, SpaceLine, 0, NORMAL, 0, CGROM);

          ucLine++;
	}
	
	//last line is end record and is fixed
	while(pTmpRec->RecRange != REC_END)
	{
		pTmpRec++;
	}
	while(pTmpRec->LineRange == LINE_MIDDLE || pTmpRec->LineRange == LINE_END)  
        { 
          pTmpRec--;
        }
	while(ucLine < M_LCDLINE)
	{
		if(pTmpRec == pCurTitleRec && MenuTitleSta == MENU_DISP)
		{
                         PrintHZ(ucLine, pTmpRec->ucTitleX, SpaceLine, 0, NORMAL, 0, CGROM);
                         PrintHZ(ucLine, pTmpRec->ucTitleX, pTmpRec->pTitle, pTmpRec->ucTitleRevX, REVERSE, pTmpRec->ucTitleRevLen, CGROM);
		}
		else
		{
                        PrintHZ(ucLine, pTmpRec->ucTitleX, pTmpRec->pTitle, pTmpRec->ucTitleRevX, NORMAL, pTmpRec->ucTitleRevLen, CGROM);
		}
		if(pTmpRec->LineRange == LINE_TOPEND || pTmpRec->LineRange == LINE_END) 
                {
                  ucLine++;
                }
		pTmpRec++;
	}
}

void RecVarToPrintStr(const REC_FRAME *pRec, unsigned char  *pPrintStr,const REC_FRAME* pCurVarRec, MENU_STA MenuVarSta, unsigned char ucVarDigit)
{
	unsigned char *pStr, ucLoop;
	unsigned char ucVar;
	char          scVar;
	unsigned int  uiVar;
	int           siVar;

	
	if(pRec->ucVarLen >= M_LCDWIDTH)
	{
		*pPrintStr = '\0';
		return;
	}
	
	switch(pRec->VarType)
	{
	case V_CHAR:
		scVar = *((char *)pRec->pVar);
		Char2Str(scVar, pRec->ucVarLen, pRec->ucVarDecLen, pPrintStr);
		break;
	case V_UCHAR:
		ucVar = *((unsigned char *)pRec->pVar);
		uChar2Str(ucVar, pRec->ucVarLen, pRec->ucVarDecLen, pPrintStr);
		break;
	case V_SHORT:
		siVar = *((short *)pRec->pVar);
		Int2Str(siVar, pRec->ucVarLen, pRec->ucVarDecLen, pPrintStr);
		break;
	case V_USHORT:
		uiVar = *((unsigned short *)pRec->pVar);
		uInt2Str(uiVar, pRec->ucVarLen, pRec->ucVarDecLen, pPrintStr);
		break;
		
	case V_STRING:
		pStr = (unsigned char *)pRec->pVar;
		*pPrintStr = *pStr;
		ucLoop = 0;
		while(*pStr != '\0')
		{
			*pPrintStr++ = *pStr++;
			if(ucLoop++ > M_LCDWIDTH) break;
		}
		*pPrintStr = '\0';
		break;
	
	case V_ENUM:
		pStr = (unsigned char *)pRec->pEnumStr[*((unsigned char *)pRec->pVar)];//µÃµ½×Ö·û´®
              
		*pPrintStr = *pStr;
		ucLoop = 0;
		while(*pStr != '\0')
		{
			*pPrintStr++ = *pStr++;
			if(ucLoop++ > M_LCDWIDTH) break;
		}
		*pPrintStr = '\0';
		break;
		
	default:*pPrintStr = '\0';
		break;
	}
}
//display screen data
//¶¥¼ÇÂ¼ µ±Ç°¼ÇÂ¼ ²Ëµ¥×´Ì¬ Êý×Ö£¨×ª»¯³É×Ö·û´®£©
void PageDataDisp(const REC_FRAME *pTopDataRec, const REC_FRAME *pCurDataRec, MENU_STA MenuDataSta, unsigned char ucDigit)
{
	const REC_FRAME *pTmpRec, *pRecTmp;
	static unsigned char  ucVarCode[M_LCDWIDTH+1];//Ã¿Ò»ÐÐ30¸ö×Ö·û
	unsigned char ucLine, ucCurInTop, *pStr;

	ucCurInTop = 0;
	pTmpRec = pTopDataRec;//´Ótop¿ªÊ¼ÏÔÊ¾
	while(pTmpRec->RecRange != REC_TOP)  pTmpRec--;// µÚÒ»ÌõÈç¹û²»ÊÇrectopÊôÐÔÖ¸Õëµ¹ÍË
	ucLine = 0;
	//µÚÒ»ÐÐ¼ÇÂ¼ÖÐÊý¾ÝµÄÏÔÊ¾
	while(ucLine == 0) 
	{
		//½«¼ÇÂ¼ÖÐÊý¾Ý²¿·Ö×ª»¯Îª×Ö·û´®ÓÃÀ´ÏÔÊ¾
		RecVarToPrintStr(pTmpRec, ucVarCode, pCurDataRec, MenuDataSta, ucDigit);
                
		pStr = ucVarCode;//ËõÐ´ÏÔÊ¾µÄ×Ö·û´®Ê×µØÖ·

		//¼ÇÂ¼ÖÐÊý¾ÝµÄÏÔÊ¾
		if(pTmpRec == pCurDataRec && MenuDataSta == MENU_EDIT)//¼ÇÂ¼Ö¸ÕëÖ¸µ½ÁËµ±Ç°¼ÇÂ¼ ÇÒ  ²Ëµ¥ÔÚ±à¼­×´Ì¬
		{
			//     ¼ÇÂ¼x                y        ×Ö·û´®µØÖ·      ·´ÏÔx         ·´ÏÔ       ·´ÏÔ³¤¶È               ·´ÏÔÄ£Ê½  
                        PrintHZ(ucLine, pTmpRec->ucVarX,pStr, pTmpRec->ucVarX, REVERSE, pTmpRec->ucVarLen, CGROM);
		}
		else
		{
                        PrintHZ(ucLine, pTmpRec->ucVarX,pStr, pTmpRec->ucVarX, NORMAL, 0, CGROM);
		}
		if(pTmpRec == pCurDataRec)  //codeµÚÒ»ÐÐ
			{ucCurInTop = 1;}

		if(pTmpRec->LineRange == LINE_TOPEND || pTmpRec->LineRange == LINE_END) 
			{ucLine++;}//ÕâÐÐÊý¾ÝÒÑ¾­ÏÔÊ¾Íê³É
		pTmpRec++;
	}
	
	if(ucCurInTop == 0)  pTmpRec = pTopDataRec;
	ucLine = M_LCDTOPSCRLINE;
	while(ucLine <= M_LCDENDSCRLINE)
	{
		//find end record
		if(pTmpRec->LineRange == LINE_TOP || pTmpRec->LineRange == LINE_TOPEND)
		{
			pRecTmp = pTmpRec;
			while(pRecTmp->LineRange == LINE_MIDDLE || pRecTmp->LineRange == LINE_TOP)  pRecTmp++;
			if(pRecTmp->RecRange == REC_END)  break;
		}
		RecVarToPrintStr(pTmpRec, ucVarCode, pCurDataRec, MenuDataSta, ucDigit);
		pStr = ucVarCode;
		if(pTmpRec == pCurDataRec && MenuDataSta == MENU_EDIT)
		{
                        PrintHZ(ucLine,pTmpRec->ucVarX, pStr, pTmpRec->ucVarX, REVERSE, pTmpRec->ucVarLen, CGROM);
		}
		else
		{
                        PrintHZ(ucLine,pTmpRec->ucVarX, pStr, pTmpRec->ucVarX, NORMAL, 0, CGROM);
		}
		if(pTmpRec->LineRange == LINE_TOPEND || pTmpRec->LineRange == LINE_END)  ucLine += 1;
		pTmpRec++;
	}
	
	//last line
	ucLine = M_LCDENDSCRLINE + 1;
	while(pTmpRec->RecRange != REC_END)
	{
		pTmpRec++;
	}
	while(pTmpRec->LineRange == LINE_MIDDLE || pTmpRec->LineRange == LINE_END)  pTmpRec--;
	while(ucLine < M_LCDLINE)
	{
		RecVarToPrintStr(pTmpRec, ucVarCode, pCurDataRec, MenuDataSta, ucDigit);
		pStr = ucVarCode;
		if(pTmpRec == pCurDataRec && MenuDataSta == MENU_EDIT)
		{
                        PrintHZ(ucLine,pTmpRec->ucVarX, pStr, pTmpRec->ucVarX, REVERSE, pTmpRec->ucVarLen, CGROM);
		}
		else
		{
                        PrintHZ(ucLine,pTmpRec->ucVarX, pStr, pTmpRec->ucVarX, NORMAL, 0, CGROM);
		}
		if(pTmpRec->LineRange == LINE_TOPEND || pTmpRec->LineRange == LINE_END) ucLine++;
		pTmpRec++;
	}

}

void MenuCtrl(void)
{

	static KEYTYPE_FRAME KeyType = KEY_NULL;       //see key.h
	static KEYSTA_FRAME  KeyStage = KEY_STA_NULL;  //see key.h
        static const REC_FRAME *PMenuTopBak = NULL;
	const REC_FRAME *pTmpRec;	// 
	unsigned int uiKeyNum = 0;
	int          siKey = 0;
	long         slVarTemp;
	unsigned char *pVuchar;
	char          *pVchar;
        unsigned short *pVuint;
        signed short *pVint;
	static unsigned char *pVstr;
	int       siMax = 0, siMin = 0;

	//----start menu remain 10 seconds
	if(StartMenu)
	{
		Menu1Ms = 0;
		if(Menu1Sec >= STARTMENU_MAX_TIME)
		{
			StartMenu = FALSE;
			Menu1Sec = STARTMENU_MAX_TIME;

			MenuSta = MENU_DISP;
			CurLine = 0;
			DigitNum = 0;
 
			//*pCurRec->pEditSta = EDIT_NULL;
			pCurRec    = PageHome;
			pScrTopRec = PageHome;
			while(pScrTopRec->LineRange == LINE_TOP || pScrTopRec->LineRange == LINE_MIDDLE) pScrTopRec++;
			pScrTopRec++;
			pTopRec    = PageHome;
		}
	}
	
	//----if no key operation for greater than M_LCDOFFTIME turn LCD back light off and jump to main menu
	if(Menu1Ms > M_LCDOFFTIME)
	{
		Menu1Ms = 0;
		
	}	
		//----flush title when current record change happened
	if(pTmpCurRec != pCurRec)
	{
	      pTmpCurRec = pCurRec;
	      
              PageTitleDisp(pScrTopRec, pCurRec, MenuSta);
	}
        
		
	if(PMenuTopBak != pTopRec)
	{
                 PMenuTopBak = pTopRec;
                 PageTitleDisp(pScrTopRec, pCurRec, MenuSta);
        }
		

	//----display audio bar,
	
	//----display data and reverse every flush cycle,
	if(ucFlushCount++ > M_FLUSHCYCLE&&pTmpCurRec == pCurRec)
	{
		ucFlushCount = 0;		
		PageDataDisp(pScrTopRec, pCurRec, MenuSta, DigitNum);//Êý¾Ý  ÏÔÊ¾
	}

	//get key
	getKey(&KeyType, &KeyStage, &uiKeyNum);//////////////////////////////
	//key not active, RET
	if(KeyStage == KEY_STA_NULL)  return;//Ã»°´¼ü¾Í²»ÓÃÔËÐÐÏÂÈ¥
	//lcdBklOn();
	Menu1Ms = 0;
	//menu control
        //ÕâÏÂÃæ¶¼ÊÇÓÐ°´¼üµÄ----------------------------------------------------
	switch(MenuSta)
	{
		case MENU_DISP:
			//wait for key release
		
                  if(KeyStage != KEY_STA_RELEASE)  return;
         
			//down
			if(KeyType == KEY_DOWN)
			{
				

				//ÏÔÊ¾ ÔÚÍ¬Ò»ÐÐÖÐµÄ¶à¸ö¼ÇÂ¼
				while(pCurRec->LineRange != LINE_END && pCurRec->LineRange != LINE_TOPEND)//ËùÔÚÐÐ»¹ÓÐ¼ÇÂ¼
				{
					pCurRec++;//¼ÇÂ¼±íµÄÏÂÒ»ÐÐ
					
					//ÕâÌõ¼ÇÂ¼ÎªÁ¬½ÓÊôÐÔ  Ò²µ½µ×ÁË
					if(pCurRec->RecType == RECTP_LINK )  return;
				}
				
				//ÖðÌõ¼ÇÂ¼ ÏÔÊ¾ Ö±µ½É¨Ãèµ½×îºóÒ»Ìõ¼ÇÂ¼
				while(pCurRec->RecRange != REC_END)
				{
					pCurRec++;//¼ÇÂ¼±íµÄÏÂÒ»ÐÐ
					//
					if(pCurRec->LineRange == LINE_TOP || pCurRec->LineRange == LINE_TOPEND)
					{
						pTmpRec = pCurRec;
						while(pTmpRec->LineRange == LINE_TOP || pTmpRec->LineRange == LINE_MIDDLE) pTmpRec++;
						if(pTmpRec->RecRange == REC_END)// ¼ÇÂ¼±íÖÐ×îºóÒ»Ìõ¼ÇÂ¼
						{
							CurLine = M_LCDENDSCRLINE + 1;
						}
						else
						{
							CurLine++;
							if(CurLine > M_LCDENDSCRLINE)
							{
								CurLine = M_LCDENDSCRLINE;
								pScrTopRec++;
								while(pScrTopRec->LineRange != LINE_TOP && pScrTopRec->LineRange != LINE_TOPEND)  pScrTopRec++;
							}
						}
					}
					if(pCurRec->RecType == RECTP_LINK)  return;
					if((pCurRec->LineRange == LINE_END || pCurRec->LineRange == LINE_TOPEND) && pCurRec->RecType != RECTP_NOCUR)  break;
				}

			 
				while(pCurRec->LineRange != LINE_TOP && pCurRec->LineRange != LINE_TOPEND)//line_middle »ò line_end
				{
					if(pCurRec->RecType == RECTP_LINK)  return;
					else pCurRec--;
				}
                                
                                if(pCurRec->RecRange == REC_END)  
                                {
                                  pCurRec--;
                                  CurLine--;
                                  return;//¼ÇÂ¼±íÀï×îºóÒ»Ìõ¼ÇÂ¼ µ½µ×ÁË
                                }
				return;
			}
			//up
			if(KeyType == KEY_UP)
			{
				if(pCurRec->RecRange == REC_TOP)  return;//±íÖÐµÄµÚÒ»Ìõ µ½¶¥ÁË
				while(pCurRec->LineRange != LINE_TOP && pCurRec->LineRange != LINE_TOPEND)
				{
					pCurRec--;
					if(pCurRec->RecType == RECTP_LINK )  return;
				}
				while(pCurRec->RecRange != REC_TOP)
				{
					pCurRec--;
					if(pCurRec->LineRange == LINE_END || pCurRec->LineRange == LINE_TOPEND)
					{
						pTmpRec = pCurRec;
						while(pTmpRec->LineRange == LINE_END || pTmpRec->LineRange == LINE_MIDDLE)  pTmpRec--;
						if(pTmpRec->RecRange == REC_TOP)
						{
							CurLine = 0;
						}
						else
						{
							CurLine--;
							if(CurLine < M_LCDTOPSCRLINE)
							{
								CurLine = M_LCDTOPSCRLINE;
								pScrTopRec = pCurRec;
								while(pScrTopRec->LineRange != LINE_TOP && pScrTopRec->LineRange != LINE_TOPEND)
								{
									pScrTopRec--;
								}
							}
						}
					}
					if(pCurRec->RecType == RECTP_LINK )  return;
					if((pCurRec->LineRange == LINE_TOP || pCurRec->LineRange == LINE_TOPEND) && pCurRec->RecType != RECTP_NOCUR)  break;
				}
				pTmpRec = pCurRec;
				while(pTmpRec->LineRange != LINE_END && pTmpRec->LineRange != LINE_TOPEND)
				{
					if(pTmpRec->RecType == RECTP_LINK )
					{
						pCurRec = pTmpRec;
						break;
					}
					pTmpRec++;
				}
				return;
			}
			//enter
			if(KeyType == KEY_ENTER)
			{
				if(pCurRec >= PageStart&& pCurRec <= PageStart+5)  StartMenu = FALSE;
			
				pTmpRec = pCurRec;  //temporary keep pCurRec, in KEY_ENTER code, never change pTmpRec untill end code
				while(1)
				{
					if(pCurRec->RecType == RECTP_NOCUR)  return;//Óöµ½²»ÄÜ±»Ñ¡ÖÐ¼ÇÂ¼ÊôÐÔ
					
					
					if(pCurRec->RecType == RECTP_LINK)//Èç¹ûÊÇÁ¬½ÓÊôÐÔµÄ¼ÇÂ¼
					{
						
						(* pCurRec->pEventFunc)();//Ö´ÐÐµ±Ç°º¯ÊýÉèÖÃÆµÂÊµÈº¯Êý
						pCurRec    = pCurRec->pLinkEnter;//È·ÈÏ µÄÁ¬½Ó
						pScrTopRec = pCurRec;
						while(pScrTopRec->LineRange == LINE_TOP || pScrTopRec->LineRange == LINE_MIDDLE) pScrTopRec++;
						pScrTopRec++;
						pTopRec    = pCurRec;
						CurLine    = 0;
						pTmpRec = pCurRec;
						while(pTmpRec->LineRange != LINE_END && pTmpRec->LineRange != LINE_TOPEND)
						{
							if(pTmpRec->RecType == RECTP_LINK )
							{
								pCurRec = pTmpRec;
								break;
							}
							pTmpRec++;
						}
						return;
					}
					if(pCurRec->RecType == RECTP_EDIT)
					{
						
							MenuSta = MENU_EDIT;
							//*pCurRec->pEditSta = EDIT_INEDIT;
							if(pCurRec->VarType == V_STRING)
							{
								pVstr = (unsigned char *)pCurRec->pVar;
								DigitNum = 0;
							}
							return;
						
					}
					if(pCurRec->LineRange == LINE_END || pCurRec->LineRange == LINE_TOPEND)  break;
					pCurRec++;
				}
				pCurRec = pTmpRec;
				return;
			}
			//escape
			if(KeyType == KEY_ESC)
			{
				if(pCurRec >= PageStart && pCurRec <= PageStart+5)
				{
					Menu1Sec = STARTMENU_MAX_TIME;
					return;
				}
				pCurRec    = pCurRec->pLinkEsc;
				pScrTopRec = pCurRec;
				if(pScrTopRec->RecRange == REC_TOP)
				{
					while(pScrTopRec->LineRange == LINE_TOP || pScrTopRec->LineRange == LINE_MIDDLE) pScrTopRec++;
					pScrTopRec++;
				}
				pTopRec    = pCurRec;
				CurLine    = 0;
				pTmpRec = pCurRec;
				while(pTmpRec->LineRange != LINE_END && pTmpRec->LineRange != LINE_TOPEND)
				{
					if(pTmpRec->RecType == RECTP_LINK)
					{
						pCurRec = pTmpRec;
						break;
					}
					pTmpRec++;
				}
				return;
			}
			break;
		case MENU_EDIT:
			//enter
			if(KeyType == KEY_ENTER)
			{
				if(KeyStage != KEY_STA_RELEASE)  return; //push active
				while(1)
				{
                                  
					if(pCurRec->RecType == RECTP_EDIT)
					{
						if(pCurRec->VarType == V_STRING)
						{
							pVstr++;
							DigitNum++;
							if(DigitNum < pCurRec->ucVarLen)  return;
						}
						//*pCurRec->pEditSta = EDIT_DONE;
						(* pCurRec->pEventFunc)();
					}
					if(pCurRec->LineRange == LINE_END || pCurRec->LineRange == LINE_TOPEND)  break;
					pCurRec++;
					if(pCurRec->RecType == RECTP_EDIT)
					{
						 
							//*pCurRec->pEditSta = EDIT_INEDIT;

							if(pCurRec->VarType == V_STRING)
							{
								pVstr = (unsigned char *)pCurRec->pVar;
								DigitNum = 0;
							}
							return;
						
					}
				}
				while(pCurRec->LineRange != LINE_TOP && pCurRec->LineRange != LINE_TOPEND)  pCurRec--;
				MenuSta = MENU_DISP;
				return;
			}
			//escape
			if(KeyType == KEY_ESC)
			{
				if(KeyStage != KEY_STA_RELEASE)  return;
				MenuSta = MENU_DISP;
				//*pCurRec->pEditSta = EDIT_ESC;
				(* pCurRec->pEventFunc)();
				while(pCurRec->LineRange != LINE_TOP && pCurRec->LineRange != LINE_TOPEND)
				{
					pCurRec--;
				}
				return;
			}
			//up or down
                        
                        if(KeyType == KEY_UP)  siKey = uiKeyNum;
			else siKey = (int)0-uiKeyNum;
                       
			switch(pCurRec->VarType)
			{
				case V_NOP:
					break;
				case V_CHAR:
					pVchar = (char *)pCurRec->pVar;
					siMax = pCurRec->siVarMax;
					siMin = pCurRec->siVarMin;
					slVarTemp = siKey + *pVchar;
					if(slVarTemp > siMax)  slVarTemp = siMax;
					if(slVarTemp < siMin)  slVarTemp = siMin;
					*pVchar = slVarTemp;
					break;
				case V_UCHAR:
					pVuchar = (unsigned char *)pCurRec->pVar;
					siMax = pCurRec->siVarMax;
					siMin = pCurRec->siVarMin;
					slVarTemp = (long int)siKey + *pVuchar;
					if(slVarTemp < siMin)  slVarTemp = siMin;
					if(slVarTemp > siMax)  slVarTemp = siMax;
					*pVuchar = slVarTemp;
					break;
				case V_SHORT:
					pVint = (signed short *)pCurRec->pVar;
					siMax = pCurRec->siVarMax;
					siMin = pCurRec->siVarMin;
					slVarTemp = siKey + *pVint;
					if(slVarTemp > siMax)  slVarTemp = siMax;
					if(slVarTemp < siMin)  slVarTemp = siMin;
					*pVint = slVarTemp;
					break;
				case V_USHORT:
					pVuint = (unsigned short *)pCurRec->pVar;
					siMax = pCurRec->siVarMax;
					siMin = pCurRec->siVarMin;
					slVarTemp = (long int)siKey + *pVuint;
					if(slVarTemp < siMin)  slVarTemp = siMin;
					if(slVarTemp > siMax)  slVarTemp = siMax;	
					*pVuint = slVarTemp;
					break;
				case V_STRING:
				
				case V_ENUM:
					pVuchar = (unsigned char *)pCurRec->pVar;
					siMax = pCurRec->siVarMax;
					siMin = pCurRec->siVarMin;
					slVarTemp =  siKey + *pVuchar;
					if(slVarTemp > siMax)  slVarTemp = siMax;
					if(slVarTemp < siMin)  slVarTemp = siMin;
					*pVuchar = slVarTemp;
					break;
				
				default:
					break;
			}
		
			break;
		default:
			break;
	}
}

















