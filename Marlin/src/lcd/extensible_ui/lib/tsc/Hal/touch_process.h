#ifndef _TOUCH_PROCESS_H_
#define _TOUCH_PROCESS_H_

#include "stdint.h"
#include "../GUI.h"


#define CMD_RDX 0xD0
#define CMD_RDY 0x90

enum
{
  KNOB_INC=1,
  KNOB_DEC,
};

#define KEY_CLICK          0x0000  //
#define KEY_DOUBLE_CLICK   0x2000  //
#define KEY_LONG_RELEASE   0x4000  //
#define KEY_LONG_CLICK     0x8000  //


void TSC_Calibration(void);
uint8_t   isPress(void);
uint16_t  KEY_GetValue(uint8_t total_rect,const GUI_RECT* menuRect);

void loopTouchScreen(void);

extern void (*TSC_ReDrawIcon)(uint8_t positon, uint8_t is_press);
extern void TS_Get_Coordinates(uint16_t *x, uint16_t *y);

#endif

