#include "../TSC_Menu.h"


MENUITEMS RGBItems = {
// title
LABEL_RGB_SETTINGS,
// icon                       label
 {{ICON_RGB_RED,              LABEL_RGB_RED},
  {ICON_RGB_GREEN,            LABEL_RGB_GREEN},
  {ICON_RGB_BLUE,             LABEL_RGB_BLUE},
  {ICON_RGB_WHITE,            LABEL_RGB_WHITE},
  {ICON_RGB_OFF,              LABEL_RGB_OFF},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};

void menuCallBackRGBSettings(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:  //Red
      storeCmd("M150 R255 U0 B0 P255");
      break;
    
    case KEY_ICON_1:  //Green
      storeCmd("M150 R0 U255 B0 P255");
      break;
    
    case KEY_ICON_2:  //Blue
      storeCmd("M150 R0 U0 B255 P255");
      break;
    
    case KEY_ICON_3:  //White
      storeCmd("M150 R255 U255 B255 P255");
      break;
    
    case KEY_ICON_4:  //Turn Off
      storeCmd("M150 R0 U0 B0 P0");
      break;
    
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default:
      break;
  }
}

void menuRGBSettings(void)
{
  menuDrawPage(&RGBItems);
  menuSetFrontCallBack(menuCallBackRGBSettings);
}


// MENUITEMS machineSettingsItems = {
// // title
// LABEL_MACHINE_SETTINGS,
// // icon                       label
//  {{ICON_RGB_SETTINGS,         LABEL_RGB_SETTINGS},
//   {ICON_SHUT_DOWN,            LABEL_SHUT_DOWN},
//   {ICON_BACKGROUND,           LABEL_BACKGROUND},
//   {ICON_BACKGROUND,           LABEL_BACKGROUND},
//   {ICON_BACKGROUND,           LABEL_BACKGROUND},
//   {ICON_BACKGROUND,           LABEL_BACKGROUND},
//   {ICON_BACKGROUND,           LABEL_BACKGROUND},
//   {ICON_BACK,                 LABEL_BACK},}
// };

MENUITEMS machineSettingsItems = {
// title
LABEL_MACHINE_SETTINGS,
// icon                       label
 {{ICON_RGB_SETTINGS,         LABEL_RGB_SETTINGS},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};


void menuCallBackMachineSettings(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      infoMenu.menu[++infoMenu.cur] = menuRGBSettings;
      break;

    // case KEY_ICON_1:
    //   storeCmd("M81");
    //   break;
    
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default:
      break;
  }
}

void menuMachineSettings(void)
{
  menuDrawPage(&machineSettingsItems);
  menuSetFrontCallBack(menuCallBackMachineSettings);
}
