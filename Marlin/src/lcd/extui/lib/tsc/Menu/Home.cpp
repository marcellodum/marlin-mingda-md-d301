#include "../TSC_Menu.h"

//1title, ITEM_PER_PAGE item
const MENUITEMS homeItems = {
// title
LABEL_HOME,
// icon                       label
 {{ICON_HOME,                 LABEL_HOME},
  {ICON_X_HOME,               LABEL_X_HOME},
  {ICON_Y_HOME,               LABEL_Y_HOME},
  {ICON_Z_HOME,               LABEL_Z_HOME},
  {ICON_EM_STOP,              LABEL_EMERGENCYSTOP},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};

void menuCallBackHome(void)
{
  KEY_VALUES key_num = menuKeyGetValue();

  switch(key_num)
  {
    case KEY_ICON_0: storeCmd(PSTR("G28"));   break;
    case KEY_ICON_1: storeCmd(PSTR("G28 X")); break;
    case KEY_ICON_2: storeCmd(PSTR("G28 Y")); break;
    case KEY_ICON_3: storeCmd(PSTR("G28 Z")); break;
    case KEY_ICON_4: flashFirmware(1); break;
    case KEY_ICON_7: infoMenu.cur--; break;
    default:break;            
  }
}

void menuHome()
{
  menuDrawPage(&homeItems);
  menuSetFrontCallBack(menuCallBackHome);
}
