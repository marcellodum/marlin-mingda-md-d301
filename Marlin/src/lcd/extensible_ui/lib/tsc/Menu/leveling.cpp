#include "../TSC_Menu.h"


#ifdef AUTO_BED_LEVELING_BILINEAR

#include "../../../../../feature/bltouch.h"

const MENUITEMS autoLevelingItems = {
// title
LABEL_ABL,
// icon                        label
 {{ICON_LEVELING,              LABEL_ABL},
  {ICON_BLTOUCH_DEPLOY,        LABEL_BLTOUCH_DEPLOY},
  {ICON_BLTOUCH_STOW,          LABEL_BLTOUCH_STOW},
  {ICON_BLTOUCH_TEST,          LABEL_BLTOUCH_TEST},
  {ICON_PROBE_OFFSET,          LABEL_PROBE_OFFSET},
  {ICON_BABYSTEP,              LABEL_BABYSTEP},
  {ICON_EM_STOP,               LABEL_EMERGENCYSTOP},
  {ICON_BACK,                  LABEL_BACK},}
};

void menuCallBackAutoBedLeveling(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      storeCmd("G28");
      storeCmd("G29");
      storeCmd("M500");
      break;
    case KEY_ICON_1:
      storeCmd("M280 P0 S10\n");
      break;
    case KEY_ICON_2:
      storeCmd("M280 P0 S90\n");
      break;
    case KEY_ICON_3:
      storeCmd("M280 P0 S120\n");
      break;
    case KEY_ICON_4:
      infoMenu.menu[++infoMenu.cur] = menuProbeOffset;
      break;
    case KEY_ICON_5:
      infoMenu.menu[++infoMenu.cur] = menuBabyStep;
      break;
    case KEY_ICON_6:
      flashFirmware(1);
      break;
    case KEY_ICON_7:
      infoMenu.cur--; break;
    default:break;
  }
}

void menuAutoLeveling()
{
  menuDrawPage(&autoLevelingItems);
  menuSetFrontCallBack(menuCallBackAutoBedLeveling);
}

#else
/*
const MENUITEMS manualLevelingItems = {
// title
LABEL_LEVELING,
// icon                        label
 {{ICON_POINT_1,               LABEL_POINT_1},
  {ICON_POINT_2,               LABEL_POINT_2},
  {ICON_POINT_3,               LABEL_POINT_3},
  {ICON_POINT_4,               LABEL_POINT_4},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BACKGROUND,            LABEL_BACKGROUND},
  {ICON_BACK,                  LABEL_BACK},}
};

void moveToLevelingPoint(uint8_t point)
{
  static const s16 pointPosition[][2] = {
    {LEVELING_POINT_1_X, LEVELING_POINT_1_Y},
    {LEVELING_POINT_2_X, LEVELING_POINT_2_Y},
    {LEVELING_POINT_3_X, LEVELING_POINT_3_Y},
    {LEVELING_POINT_4_X, LEVELING_POINT_4_Y},
  };
  if(coordinateIsClear() == false)
  {
    storeCmd("G28\n");
  }
  storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_MOVE_Z, LEVELING_POINT_Z_FEEDRATE);
  storeCmd("G0 X%d Y%d F%d\n", pointPosition[point][0], pointPosition[point][1], LEVELING_POINT_XY_FEEDRATE);
  storeCmd("G0 Z%.3f F%d\n", LEVELING_POINT_Z, LEVELING_POINT_Z_FEEDRATE);
}

void menuCallBackManualLeveling(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0: moveToLevelingPoint(0); break;
    case KEY_ICON_1: moveToLevelingPoint(1); break;
    case KEY_ICON_2: moveToLevelingPoint(2); break;
    case KEY_ICON_3: moveToLevelingPoint(3); break;
    case KEY_ICON_7:
      infoMenu.cur--; break;
    default:break;
  }
}

void menuManualLeveling()
{
  menuDrawPage(&manualLevelingItems);
  menuSetFrontCallBack(menuCallBackManualLeveling);
}*/

#endif
