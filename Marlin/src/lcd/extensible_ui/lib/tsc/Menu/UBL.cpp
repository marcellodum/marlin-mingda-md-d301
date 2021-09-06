#include "UBL.h"
#include "../TSC_Menu.h"

//1 title, ITEM_PER_PAGE items(icon+label) 
MENUITEMS ublItems = {
//title
  LABEL_UBL,
//icon                        label
 {{ICON_LEVELING, LABEL_START},
  {ICON_EEPROM_SAVE, LABEL_SAVE},
  {ICON_EEPROM_RESTORE, LABEL_LOAD},
  {ICON_BACKGROUND, LABEL_BACKGROUND},
  {ICON_BACKGROUND, LABEL_BACKGROUND},
  {ICON_BACKGROUND, LABEL_BACKGROUND},
  {ICON_BACKGROUND, LABEL_BACKGROUND},
  {ICON_BACK,       LABEL_BACK},}
};

// Start UBL process
void ublStart(void)
{
  storeCmd("G28\n");
  storeCmd("G29 P1\n");
  // Run this multiple times since it only fills some missing points, not all.
  storeCmd("G29 P3\n");
  storeCmd("G29 P3\n");
  storeCmd("G29 P3\n");
  storeCmd("M500\n");
  storeCmd("M118 A1 UBL Completed\n");
}

void menuCallBackUBL(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      ublStart();
      break;
    case KEY_ICON_1:
        //menuUBLSave();
      break;
    case KEY_ICON_2:
        //menuUBLLoad();
      break;
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    default :
      break;
  }
}

void menuUBL()
{
  menuDrawPage(&ublItems);
  menuSetFrontCallBack(menuCallBackUBL);
}
