#ifndef _PREHEATMENU_H_
#define _PREHEATMENU_H_

//                       PLA      PETG       ABS     "CUSTOM1" "CUSTOM2"
#define PREHEAT_BED      {60,      70,       100,       55,       55}
#define PREHEAT_HOTEND   {200,     250,      230,       200,      200}

typedef enum
{
  BOTH = 0,
  BED_PREHEAT = 1,
  NOZZLE0_PREHEAT = 2,
}TOOLPREHEAT;


void menuPreheat(void);

#endif

