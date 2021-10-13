#include "../TSC_Menu.h"

MENUITEMS screenSettingsItems = {
// title
LABEL_SCREEN_SETTINGS,
// icon                       label
 {{ICON_ROTATE_UI,            LABEL_ROTATE_UI},
  {ICON_TOUCHSCREEN_ADJUST,   LABEL_TOUCHSCREEN_ADJUST},
  {ICON_LANGUAGE,             LABEL_LANGUAGE}, 
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};

#ifdef BEEPER_PIN // Speaker
  #define BUZZER_KEY_INDEX KEY_ICON_3

  #define ITEM_SILENT_NUM 2
  const ITEM itemSilent[ITEM_SILENT_NUM] = {
  // icon                       label
    {ICON_SILENT_OFF,           LABEL_SILENT_OFF},
    {ICON_SILENT_ON,            LABEL_SILENT_ON},
  };
  const  uint8_t item_silent[ITEM_SILENT_NUM] = {0, 1};
  static uint8_t item_silent_i = 0;
#endif

  
void menuCallBackScreenSettings(void)
{ 
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      infoSettings.rotate_ui = !infoSettings.rotate_ui;
      LCD_RefreshDirection();
      TSC_Calibration();
      menuDrawPage(&screenSettingsItems);
      break;
    
    case KEY_ICON_1:
      TSC_Calibration();
      menuDrawPage(&screenSettingsItems);
      break;
    
    case KEY_ICON_2: 
      infoSettings.language = (infoSettings.language + 1) % LANGUAGE_NUM;
      menuDrawPage(&screenSettingsItems);
      break;
    
    #ifdef BEEPER_PIN
    case BUZZER_KEY_INDEX:
      item_silent_i = (item_silent_i + 1) % ITEM_SILENT_NUM;                
      screenSettingsItems.items[key_num] = itemSilent[item_silent_i];
      menuDrawItem(&screenSettingsItems.items[key_num], key_num);
      infoSettings.silent = item_silent[item_silent_i];
      break;
    #endif
    
    case KEY_ICON_7:
      infoMenu.cur--;
      break;
    
    default:
      break;
  }
}

void menuScreenSettings(void)
{
  #ifdef BEEPER_PIN
    for(uint8_t i=0; i<ITEM_SILENT_NUM; i++)
    {
      if(infoSettings.silent == item_silent[i])
      {
        item_silent_i = i;
        screenSettingsItems.items[BUZZER_KEY_INDEX] = itemSilent[i];
      }
    }
  #endif  
  menuDrawPage(&screenSettingsItems);
  menuSetFrontCallBack(menuCallBackScreenSettings);
}

