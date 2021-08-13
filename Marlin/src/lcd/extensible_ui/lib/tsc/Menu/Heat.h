#ifndef _HEAT_H_
#define _HEAT_H_

#include "stdint.h"

#define HEAT_DISPLAY_ID  {"Bed",  "T0",      "T1",      "T2",      "T3",      "T4",      "T5"}
extern const char* const heatDisplayID[];

void heatSetCurHeaterIndex(uint8_t index);
void menuHeat(void);

#endif
